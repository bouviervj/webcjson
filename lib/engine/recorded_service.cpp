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
#include "recorded_service.h"


#include <jsonxx.h>
#include <sstream>
#include <files.h>

using namespace jsonxx;

namespace webcjson {

	std::string recorded_service::getArgName(int i){
		std::ostringstream oStream;
		oStream << "arg" << i;
		return oStream.str();
	}
	
	void recorded_service::decodePostBody(http_context* iContext, std::string& iFunction, std::vector<std::string>& iArguments){
	
		jsonxx::Object aObject;
	     if (aObject.parse(iContext->_body)) {

			if (aObject.has<String>("_function")) {
				iFunction = aObject.get<String>("_function");
			}

			int nArgs = aObject.get<float>("_nargs");
			//printf("nargs:%d\n",nArgs);	

			if (aObject.has<float>("_nargs")) {
				//iContext->_key_value["_nargs"] = 
			
				for (int i=0; i<nArgs; i++) {
					//printf("extract:%d\n",i);
					std::string aArg = aObject.get<Value>(getArgName(i)).json(); // TODO manage exception
					//printf("value:%s\n",aArg.c_str());
					iArguments.push_back(aArg);	
				}
			
			}

		} else {

			printf("Can't parse entry data");

		}
		
	}

	void recorded_service::proceed(http_context* iContext){

		std::string aResult;
		
		std::string aCalledFunction;
		std::vector<std::string> aArguments;
		decodePostBody(iContext,aCalledFunction,aArguments);

		bool aReturnJavascript = true;

		if (aCalledFunction!="") {

			if (!execute(aCalledFunction, aArguments, aResult)) { 
				iContext->_reply_code = 200;
			} else { // ERROR
				iContext->_reply_code = 400; // Bad Request
				aReturnJavascript = false;
			}

		} else {

			std::cout << "Empty function name, reply with service definition." << std::endl;
			returnDefinition(aResult);
			iContext->_reply_code = 200;

		}

		if (aReturnJavascript) {
			iContext->_extension_reply = files::get_file_extension("*.js");
			iContext->_body_reply = iContext->_params["callback"]+"({\"Response\":"+aResult+"});";
		} else {
			iContext->_extension_reply = files::get_file_extension("*.html");
			iContext->_body_reply = "<html><head><title>ERROR</title></head><body><pre>"+aResult+"</pre></body></html>";
		}
		
	}	
	

}
