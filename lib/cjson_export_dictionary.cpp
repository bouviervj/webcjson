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
#include "cjson_export_dictionary.h"

#include "cjson_dictionary.h"

namespace webcjson {

	export_dictionary::export_dictionary(const std::string& iGrammarFileName){

		_internalDictionary = (void*) new cjson::dictionary(iGrammarFileName);

	}

	std::string export_dictionary::toJson(const std::string& iObjectName, const void* iPointer){

		cjson::field::field* aDataStruct = ((cjson::dictionary*) _internalDictionary)->getDataStruct(iObjectName);

		if (aDataStruct) { // Here we can decode the object
			std::ostringstream aStream;
			aDataStruct->toJson(aStream, iPointer);
			return aStream.str();
		}
		
		printf("Unable to find resource: %s\n",iObjectName.c_str());
		return "";

	}
	
	void* export_dictionary::fromJson(const std::string& iObjectName, const std::string& iJson){

		cjson::field::field* aDataStruct = ((cjson::dictionary*) _internalDictionary)->getDataStruct(iObjectName);

		if (aDataStruct) {
			return aDataStruct->fromJson(iJson);
		}

		return 0;

	}


}
