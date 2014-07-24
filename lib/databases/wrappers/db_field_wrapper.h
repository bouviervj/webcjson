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
#ifndef __DB_FIELD_WRAPPER_H__
#define __DB_FIELD_WRAPPER_H__

#include <db_driver.h>
#include <cjson_field.h>

namespace dbcjson {

	class db_field_wrapper {

		public: 

			db_field_wrapper(cjson::field::field* iField){
				 _innerField = iField;
			} 

			virtual void prepare(db_driver* iDriver) = 0;

			virtual bool isPrepared() = 0;

			virtual void store(db_driver* iDriver, void* iDataPointer){
				abort();
			}


		protected:

			cjson::field::field* _innerField;	

	};


}


#endif
