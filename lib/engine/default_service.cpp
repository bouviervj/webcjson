/*
 * Copyright 2014 BOUVIER-VOLAILLE Julien
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "default_service.h"

#include <iostream>
#include <stdio.h>

#include "../utils/files.h"
#include "../utils/advanced_string.h"

#include "http_cache.h"

namespace webcjson {

	
	int default_service::resolve(const std::string& url, std::string& iResolved){
			
		std::string aUrl = url;	
		if (adv::startWidth(url,"/")) {
			aUrl = "."+url;
		}
		
		std::string expected = _htdocs + aUrl;
		std::string aFullPath;
		if (files::get_real_path(expected.c_str(),aFullPath)) { // valid path ?

			int res=files::isDirectory(aFullPath);
			iResolved = aFullPath;
			if (res==1) { // Directory  
				iResolved = aFullPath+"/index.html";

				if (!files::exists(iResolved)) {
					iResolved = aFullPath+"/index.htm";
				}

			}

			return 1;

		}

		return 0; // return 0 not valid path, unable to resolve

	}

	bool default_service::init_security(){
	
		std::string aAuthorized = http_properties::getInstance().get("authorized");
		std::vector<std::string> aPaths;
		adv::split(aAuthorized,',', aPaths);

		_authorized_paths.clear();
		for (int i=0; i<aPaths.size(); i++) {
			std::string aPath;
			if (files::get_real_path(aPaths[i],aPath)) {
				_authorized_paths.push_back(aPath);
			}
		}

	}

	bool default_service::validate_security(const std::string& iPath){
	
		for (int i=0; i<_authorized_paths.size(); i++) {

			printf("validate %s %s\n",iPath.c_str(),_authorized_paths[i].c_str());	

			if (adv::startWidth(iPath,_authorized_paths[i].c_str())) {
				return true;
			}
		}
		return false;		

	}

	void default_service::proceed(http_context* iContext){

		std::cout << "URL:" << iContext->_url << std::endl;

		std::string iFileName;
	 	if (resolve(iContext->_url,iFileName)) {
		
			if (validate_security(iFileName)) {

				if (files::exists(iFileName)) {
					iContext->_reply_code = 200;
					iContext->_body_reply = files::get_file_contents(iFileName); // Here takes files from default user directory

					if (!_cache.manage(iContext)) { // replies 304 if hit the cache
						iContext->_extension_reply = files::get_file_extension(iFileName);
					}  
				
				} else {
					iContext->_reply_code = 404; // Error not found
				}	

			} else {
				iContext->_reply_code = 401; // Unauthorized	
			}	
		
		} else {
			iContext->_reply_code = 404; // Error not found
		}

	}	

}
