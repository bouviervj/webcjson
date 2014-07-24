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
#ifndef __SERVICE_LOADER_H__
#define __SERVICE_LOADER_H__

#include <engine.h>
#include <vector>

namespace webcjson {

	class service_loader {

		public:

			static service_loader& getInstance();

			void addEngine(engine* iEngine){
				_engines.push_back(iEngine);
			}

			void load(const std::string& iURI, service* iService);			
			
			void loadServices();

		private: 

			service_loader(){}

			service_loader(service_loader const&);         
       		void operator=(service_loader const&); 

			std::vector<engine*> _engines;

	};


}

#endif
