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
#ifndef __SERVICE_RECORDER_H__
#define __SERVICE_RECORDER_H__

#include <map>
#include "http_context.h"
#include "service.h"
#include "default_service.h"
#include "../utils/Properties.h"

namespace webcjson {

	class service_recorder {

		public:

			service_recorder();

			void record(const std::string& iURI, service* iService);

			void run(http_context* ioContext);

			void loadServices();

		private:

			void extractParams(http_context* ioContext);			

			std::map<const std::string,service*> _services;
			service* _default_service;	

	};


}

#endif
