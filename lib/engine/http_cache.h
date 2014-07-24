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
#ifndef __HTTP_CACHE_H__
#define __HTTP_CACHE_H__

#include <stdio.h>
#include <time.h>
#include <ctime>
#include <map>
#include <string>

#include "http_context.h"

namespace webcjson {
	
	
	struct http_hash {

		static unsigned long hash(const char* iData, unsigned long iSize){
		
		    unsigned long mix = 0;
		    unsigned long count = 0;	
		  
		    const unsigned long mulp = 2654435789;

		    mix ^= 104395301;

		    while(count<iSize) {
			   mix += (iData[count] * mulp) ^ (mix >> 23);
			   count++;
		    }

		    return mix ^ (mix << 37);				

		}

		static void hash(std::string& ioStr,unsigned long iHash){
			char buf[10];
			int len = snprintf(buf,10,"%0lx",iHash);
			ioStr = std::string(buf,9);
		}

		static void hash(std::string& ioStr,const char* iData, unsigned long iSize){
			hash(ioStr,hash(iData,iSize));
		}		

	};

	class http_elem {
		public:
			time_t expires_time;
	};

	class http_cache {

		public: 

			http_cache(){
			}

			bool manage(http_context* ioContext);

			http_elem* hit(const unsigned long iHash);

			void add(const unsigned long iHash);

			static void formattime(std::string& ioStrTime, const long iRawtime, const long iShift = 0);
			static void gettime(std::string& ioStrTime, const long iShift = 0);
			static void dump_time();

		private:

			std::map<unsigned long,http_elem> _map;

	};

}

#endif
