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
#ifndef __CJSON_EXPORTED_DICTIONARY_H__
#define __CJSON_EXPORTED_DICTIONARY_H__

#include <string.h>
#include <string>
#include <typeinfo>
#include <./utils/demangle.h>

/*
namespace cjson {
	class dictionary;
}
*/

namespace webcjson {

	class export_dictionary {

		public:

			export_dictionary(const std::string& iGrammarFileName);

			std::string toJson(const std::string& iObjectName, const void* iPointer);
			void* fromJson(const std::string& iObjectName, const std::string& iJson);

			template<class T>
			std::string toJson(const T* aPointerToObject);

			template<class T>
			T* fromJson(const std::string& iJson);

		private:

			/*cjson::dictionary* */void* _internalDictionary;

	};

	template<class T>
	std::string export_dictionary::toJson(const T* aPointerToObject) {	

		std::string aClassName = typeid(*aPointerToObject).name();	
		char aBuffer[2048];
		demangle(aBuffer, 2048, aClassName.c_str());
		
		return toJson(std::string(aBuffer), aPointerToObject);

	}

	template<class T>
	T* export_dictionary::fromJson(const std::string& iJson){
		
		T* aObject;
		std::string aClassName = typeid(aObject).name();	

		char aBuffer[2048];
		demangle(aBuffer, 2048, aClassName.c_str());
		aBuffer[strlen(aBuffer)-1] = 0;

		return reinterpret_cast<T*> (fromJson(std::string(aBuffer),iJson));

	}

}

#endif 
