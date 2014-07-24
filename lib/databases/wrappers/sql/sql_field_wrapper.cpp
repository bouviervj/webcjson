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

#include "sql_field_wrapper.h"


namespace dbcjson {

	void cmp(const dbcjson_dm_desc_t& aBase, 
		    const dbcjson_dm_desc_t& aComp, 
		    dbcjson_dm_desc_t& iInsert, 
		    dbcjson_dm_desc_t& iDelete){

		typedef dbcjson_dm_desc_t::const_iterator it_type;
		for(it_type iterator = aBase.begin(); iterator != aBase.end(); iterator++) {
		    it_type iterator2 = aComp.find(iterator->first);
		    if (iterator2!=aComp.end()) {
				if (!(iterator2->second==iterator->second)) {
					iDelete[iterator->first] = iterator->second;
					iInsert[iterator->first] = iterator2->second;
				}
		    } else {
				iDelete[iterator->first] = iterator->second;
		    }
   		}

		for(it_type iterator = aComp.begin(); iterator != aComp.end(); iterator++) {
		    it_type iterator2 = aBase.find(iterator->first);
		    if (iterator2==aBase.end()) {
				iInsert[iterator->first] = iterator->second;
		    }
   		}

	}

}
