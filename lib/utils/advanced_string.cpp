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
#include "advanced_string.h"

#include <string.h>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <sstream>
#include <vector>

namespace adv {

	bool	startWidth(const std::string& iStr, const char* iPrefix){
	   	if (!iStr.compare(0, strlen(iPrefix), iPrefix)) {
			return true;
		}
		return false;
	}

	bool endsWith(std::string const &fullString, const char* ending) {
	    int strEnd = strlen(ending);
	    if (fullString.length() >= strEnd) {
		   return (0 == fullString.compare( (fullString.length() - strEnd), strEnd, ending));
	    } else {
		   return false;
	    }
	}
	
	bool contains(const std::string& iStr1, const char* iStr2){
		return contains(iStr1,std::string(iStr2));
	}

	bool contains(const std::string& iStr1, const std::string& iStr2){
		if (iStr1.find(iStr2) != std::string::npos) {
		    return true;
		}
		return false;
	}

	// trim from start
	std::string &ltrim(std::string &s) {
		   s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		   return s;
	}

	// trim from end
	std::string &rtrim(std::string &s) {
		   s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		   return s;
	}

	// trim from both ends
	std::string &trim(std::string &s) {
		   return ltrim(rtrim(s));
	}

	std::vector<std::string> &split(const std::string &s,const char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
		   elems.push_back(item);
	    }
	    return elems;
	}

	std::vector<std::string> split(const std::string &s,const char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}

}
