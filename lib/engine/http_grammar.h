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
#ifndef __HTTP_GRAMMAR_H__
#define __HTTP_GRAMMAR_H__

#include <cjson_export_dictionary.h>

namespace webcjson {
	
	class http_grammar {

		public:

			static http_grammar& getInstance();

			export_dictionary* getDictionary(){
				return _dictionary;
			}
			

		private:

			http_grammar();

			export_dictionary* _dictionary;

	};

}

#endif
