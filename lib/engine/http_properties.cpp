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
#include "http_properties.h"

namespace webcjson {

	std::string http_properties::_filename;

	http_properties::http_properties(const std::string& iFileName){
		_properties = new conf::Properties(iFileName);
	}

	void http_properties::setPropertyFile(const std::string& iFileName){
		_filename = iFileName;
	}

	http_properties& http_properties::getInstance(){
		 static http_properties instance(_filename);
		 return instance;
	}
	
	const std::string& http_properties::get(const std::string& iKey){
		return (*_properties)[iKey];
	}

}
