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
#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_

#include <string>
#include <map>
#include <vector>

namespace conf {

	class Properties {

	private :

		std::vector<std::string> _keys;
		std::map<std::string, std::string> _properties;

	public :

		Properties(const char* iFileName);

		Properties(const std::string& iFileName);

		std::string& operator[](const std::string& iKey)
	  	{
		 return _properties[iKey] ;
	  	}

		std::string& operator[](const char* iKey)
	  	{
		 return _properties[iKey] ;
	  	}

		int size(){
		 return _properties.size();		
		}

		std::vector<std::string> keys(){
			return _keys;
		}

	private :

		void init();

		std::string _filename;
		

	};

}

#endif 

