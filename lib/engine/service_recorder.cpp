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
#include "service_recorder.h"

#include <utils.h>
#include <iostream>
#include <files.h>

#include <dlfcn.h>

namespace webcjson {

	service_recorder::service_recorder(){

		_default_service = new default_service();		

	}	

	void service_recorder::record(const std::string& iURI, service* iService){
			
		_services[iURI] = iService;		

	}

	void service_recorder::extractParams(http_context* ioContext) {

		// Here we have to rebuild the url
		std::vector<std::string> aElems;
		split(ioContext->_url, '?', aElems);
		std::cout << "URL:" << ioContext->_url << std::endl;
		ioContext->_url = aElems[0];
		std::cout << "EXTR URL:" << ioContext->_url <<std::endl;
	
		if (aElems.size()>1){

			std::string aParams = aElems[1];
			std::vector<std::string> aSplParams;
			split(aParams,'&', aSplParams);

			for (int i=0; i<aSplParams.size(); i++){
				std::cout << "param:" << aSplParams[i] << std::endl;
			
				std::vector<std::string> aKeyValue;
				split(aSplParams[i],'=', aKeyValue);

				if (aKeyValue.size()>1) {
					ioContext->_params[aKeyValue[0]] = aKeyValue[1];
				}

			}


		}

	}

	void service_recorder::run(http_context* ioContext){

		extractParams(ioContext); 

		if (_services.find(ioContext->_url)!=_services.end()) {
			_services[ioContext->_url]->proceed(ioContext);
		} else {
			_default_service->proceed(ioContext);
		}

	}


}
