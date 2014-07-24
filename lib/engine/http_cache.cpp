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
#include "http_cache.h"

#define EXPIRE_TIME 200

namespace webcjson {

	bool http_cache::manage(http_context* ioContext){

		unsigned long aHash = http_hash::hash(ioContext->_body_reply.c_str(), ioContext->_body_reply.size());

		http_elem* aElem = hit(aHash);
		if (aElem) {
			ioContext->_reply_code = 304;
			ioContext->_body_reply = ""; // don't reply body
			return true;
		} 

		// Add enty in cache
		add(aHash);

		// comput Etag
		std::string eTag;
		http_hash::hash(eTag,aHash);
		ioContext->_key_value_reply["Etag"]=eTag;

		// Setup expiry time
		std::string aTime;
		http_cache::gettime(aTime);
		ioContext->_key_value_reply["Date"]=aTime;
		http_cache::gettime(aTime,EXPIRE_TIME);
		ioContext->_key_value_reply["Expires"]=aTime;
		ioContext->_key_value_reply["Cache-Control"]="public, max-age=200";

		return false;

	}

	http_elem* http_cache::hit(const unsigned long iHash){
		if (_map.find(iHash)!=_map.end()) {
				
			time_t rawtime;
			time ( &rawtime );
			
			if (_map[iHash].expires_time<rawtime) {
				_map.erase(iHash);
				return 0;
			} else {
				return &_map[iHash];
			}
		}
		return 0;
	}

	void http_cache::add(const unsigned long iHash){

		http_elem aElem;
		time (&aElem.expires_time);
		aElem.expires_time+=EXPIRE_TIME;
		_map[iHash]=aElem;

	}

	void http_cache::formattime(std::string& ioStrTime, const long iRawTime, const long iShift){
			
		struct tm * timeinfo;
	     long aRawtime = iRawTime;

		aRawtime+=iShift;
		timeinfo = gmtime ( &aRawtime );
		 
		char buffer[100];
		size_t len = strftime(buffer, 2048, "%a, %d %b %G %H:%M:%S GMT", timeinfo );

		ioStrTime = std::string(buffer,len);

	}

	void http_cache::gettime(std::string& ioStrTime, const long iShift){
			
		time_t rawtime;
		time ( &rawtime );
		
		formattime(ioStrTime, rawtime, iShift);

	}

	void http_cache::dump_time(){
	
		 std::string iTime;
		 gettime(iTime);
		 printf ( "Current time and date: %s \n", iTime.c_str() );

	}

}
