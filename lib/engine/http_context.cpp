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
#include "http_context.h"
#include "http_cache.h"

#include <iostream>
#include <sstream> 
#include "../lib/utils/advanced_string.h"


	void http_context::addCookie(const std::string& iName, 
				const std::string& iValue, 
				const std::string& iDomain, 
				const std::string& iPath, 
				const long iExpires, 
				bool iSecure, 
				bool iHttpOnly){

		http_cookie aCookie;
		aCookie._name = iName;
		aCookie._value = iValue;
		aCookie._path = iPath;

		time ( &aCookie._expiration_time );
		aCookie._expiration_time+=iExpires;
		
		aCookie._secure = iSecure;
		aCookie._httpOnly = iHttpOnly;

		aCookie._new = true;
		aCookie._remove = false;

		_cookies[iName] = aCookie;
	}

	http_cookie* http_context::getCookie(const std::string& iKey){

		if (_cookies.find(iKey)==_cookies.end()) {
			return 0;
		}
		return &_cookies[iKey];

	}

	void http_context::removeCookie(const std::string& iName){
		_cookies[iName]._expiration_time = 0;
		_cookies[iName]._remove = true;
	}

	void http_context::formatCookies(std::ostringstream& ioFormatted){

		bool defined = false;
		std::ostringstream ioCookies;
		ioCookies << "Set-Cookie: " ; 
		typedef std::map<std::string,http_cookie>::iterator it_type;
		for(it_type iterator = _cookies.begin(); iterator != _cookies.end(); iterator++) {
		    if (iterator->second._new) {	
			  ioCookies << iterator->second._name;	    	  
			  ioCookies << "=" << iterator->second._value.c_str() << "; Path=" << iterator->second._path.c_str() << ";";
			  std::string iDate;
			  webcjson::http_cache::gettime(iDate, iterator->second._expiration_time);
			  ioCookies << " Expires: " << iDate.c_str() <<";";
			  if (iterator->second._secure) {
				ioCookies << " Secure;";
			  }
			  if (iterator->second._httpOnly) {
				ioCookies << " HttpOnly;";
			  }
			  iterator->second._new = false;
			  defined = true;
		    } 	
		}
		if (defined) ioFormatted << ioCookies.str() << std::endl;

	}

	void http_context::extractCookies(){

		if (_key_value.find("Cookie")!=_key_value.end()) {

			std::string aCDef = _key_value["Cookie"];
			std::vector<std::string> aList;
			adv::split(aCDef,';', aList);			
			
			for (int i=0; i<aList.size(); i++) {
				printf("Def Cookie:%s\n",aList[i].c_str());
				if (adv::contains(aList[i],"=")) {

					std::vector<std::string> aKeyValue;					
					adv::split(aList[i],'=', aKeyValue);
					
					std::string aKey = adv::trim(aKeyValue[0]);
					std::string aValue = adv::trim(aKeyValue[1]);
					addCookie(aKey, aValue, "/", "", 0, false, false);
					_cookies[aKey]._new = false;
										
				}
			}

		}

	}

	void http_context::deleteCookies(){
		_cookies.clear();
	}

	void http_context::dumpCookies(){
		typedef std::map<std::string,http_cookie>::iterator it_type;
		for(it_type iterator = _cookies.begin(); iterator != _cookies.end(); iterator++) {
			printf("Defined Cookie:#%s# %s \n", iterator->first.c_str() , iterator->second._value.c_str());
		}
	}


