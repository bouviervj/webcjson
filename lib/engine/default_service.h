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
#ifndef __DEFAULT_SERVICE_H__
#define __DEFAULT_SERVICE_H__

#include "service.h"
#include "../utils/advanced_string.h"

#include "http_properties.h"
#include "http_cache.h"
#include <vector>

namespace webcjson {

	class default_service : public service {

		public: 

			default_service(): service() {

				_htdocs = webcjson::http_properties::getInstance().get("htdocs");
				if (!adv::endsWith(_htdocs, "/")) {
					_htdocs += "/";	
				}

				init_security();

			}

			virtual void proceed(http_context* iContext);

		private:

			int resolve(const std::string& url, std::string& iResolved);
			bool init_security();
			bool validate_security(const std::string& iPath);

			std::string _htdocs;

			std::vector<std::string> _authorized_paths;

			http_cache _cache;

	};


}

#endif
