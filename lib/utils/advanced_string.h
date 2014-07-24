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
#ifndef __ADVANCED_STRING_H__
#define __ADVANCED_STRING_H__

#include <string>
#include <vector>

namespace adv {

	bool	startWidth(const std::string& iStr, const char* iPrefix);

	#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
		   ( std::ostringstream() << std::dec << x ) ).str()

	std::string &ltrim(std::string &s);

	std::string &rtrim(std::string &s);

	std::string &trim(std::string &s);

	std::vector<std::string> &split(const std::string &s,const char delim, std::vector<std::string> &elems);

	std::vector<std::string> split(const std::string &s,const char delim);

	bool endsWith(std::string const &fullString, const char* ending);

	bool contains(const std::string& iStr1, const char* iStr2);
	bool contains(const std::string& iStr1, const std::string& iStr2);
}


#endif
