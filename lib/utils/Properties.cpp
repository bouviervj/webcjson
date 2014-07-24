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
#include "Properties.h"

#include <fstream>

#include "advanced_string.h"

namespace conf {

	Properties::Properties(const char* iFileName){
		
		_filename = std::string(iFileName);

		init();
	}


	Properties::Properties(const std::string& iFileName){
		
		_filename = iFileName;

		init();
	}

	void Properties::init(){
		
		std::ifstream infile(_filename.c_str());

		std::string line;
		while (std::getline(infile, line))
		{
			std::vector<std::string>	elems;
			elems = adv::split(line, '=', elems);	   

			if (elems.size()==2) {
				std::string aKey = adv::trim(elems[0]);
				_keys.push_back(aKey);
				_properties[aKey] = elems[1];
				//printf("reading setup: %s set to %s \n" , elems[0].c_str(), elems[1].c_str() );
			} else if (elems.size()==1) {
				std::string aKey = adv::trim(elems[0]);
				_keys.push_back(aKey);
				_properties[aKey] = std::string("");
				//printf("reading setup: %s set to nothing. \n" , elems[0].c_str() );
			}			
 
		}

	}	


}

