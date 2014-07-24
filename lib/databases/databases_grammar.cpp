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
#include "databases_grammar.h"

#include "databases_properties.h"

namespace webcjson {

	databases_grammar& databases_grammar::getInstance(){
		 static databases_grammar instance;
		 return instance;
	}
	
	databases_grammar::databases_grammar(){
		
		_dictionary = new webcjson::export_dictionary(databases_properties::getInstance().get("databases.grammar")); // TODO load once grammar ?

	}


}
