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
#ifndef __RECORDED_SERVICE_H__
#define __RECORDED_SERVICE_H__

#include "service.h"
#include "service_loader.h"
#include <iostream>
#include <vector>

class strless {
   public:
      bool operator() (const std::string & first, const std::string & second ) const  {
         return first < second;
      }
};

namespace webcjson {



	class recorded_service : public service {

		public:

			recorded_service(const std::string& iURLToRecord) : service() {

				std::cout << " Declaration: " << iURLToRecord << "\n\r" ;

				webcjson::service_loader::getInstance().load(iURLToRecord,this);

			}

			virtual void proceed(http_context* iContext);

			virtual int execute(const std::string& iFunction, const std::vector<std::string>& iArgs, std::string& ioResult) = 0;

			virtual void returnDefinition(std::string& ioDefinition) = 0;

		private:

			std::string getArgName(int i);

			void decodePostBody(http_context* iContext, std::string& iFunction, std::vector<std::string>& iArguments);

	};


	template<typename MODULE>
	class module_service :  public recorded_service {

		public: 

			module_service(const std::string& iURLToRecord) : recorded_service(iURLToRecord) {}
			
		protected:

			MODULE _module;

	};

	template<typename MODULE_SESSION>
	class module_session_service :  public recorded_service {

		public: 

			module_session_service(const std::string& iURLToRecord) : recorded_service(iURLToRecord) {}
			
			virtual void proceed(http_context* iContext){

				dumpSession();

				//Here decode context
				std::string aHash;
				std::string aSessionContext;

				bool createCookie = false;
				http_cookie* aCookie = iContext->getCookie("webcjson_sessionid");
				if (!aCookie) {
					createCookie = true;
				} else {
					
					printf("Cookie hash:#%s# %d\n",aCookie->_value.c_str(), (unsigned int) aCookie->_value.size());
					if (_sessions_contexts.find(aCookie->_value)!=_sessions_contexts.end()) {
						aHash = aCookie->_value;
						createCookie = false;
					} else {
						createCookie = true;
					}
				}

				if (createCookie) {

					std::string aIPHash;
					http_hash::hash(aIPHash, iContext->_ip.c_str(), iContext->_ip.size());

					std::string aHashed;
					http_cache::gettime(aHashed);					
					http_hash::hash(aHash, aHashed.c_str(), aHashed.size());
			
					aHash = aHash+"_"+aIPHash;
					printf("Create session hash:%s %d\n",aHash.c_str(), (unsigned int) aHash.size());

					iContext->addCookie("webcjson_sessionid", aHash , "", "/", 200, false, true);

					_sessions_contexts[aHash] = "";
					aCookie = iContext->getCookie("webcjson_sessionid");
					_module.initSession();

				} else {
				
					printf("Session hash:%s\n",aCookie->_value.c_str());				
					aSessionContext = _sessions_contexts[aCookie->_value];
					printf("Retrieved Context:%s\n",aSessionContext.c_str());
					_module.setContext(aSessionContext);

				}

				recorded_service::proceed(iContext);

				//Here encode context
				_module.extractContext(aSessionContext);
				_sessions_contexts[aHash] = aSessionContext;
				printf("Saved Context: #%s# %s\n",aHash.c_str(), aSessionContext.c_str());

				_module.reset();

			}

			void dumpSession(){
				typedef std::map<std::string,std::string>::iterator it_type;
				for(it_type iterator = _sessions_contexts.begin(); iterator != _sessions_contexts.end(); iterator++) {
					printf("Defined Session:#%s# %s \n", iterator->first.c_str() , iterator->second.c_str());
				}
			}


		protected:

			MODULE_SESSION _module;

			std::map<std::string, std::string, strless> _sessions_contexts;
			
	};


}

#define ADD_RECORDED_SERVICE(FIELDCLASS) static const FIELDCLASS* _instance;
#define CREATE_RECORDED_SERVICE(FIELDCLASS,URL) const FIELDCLASS* FIELDCLASS::_instance = new FIELDCLASS(URL); 


#endif
