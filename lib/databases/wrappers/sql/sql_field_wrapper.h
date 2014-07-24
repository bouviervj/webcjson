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
#ifndef __SQL_FIELD_WRAPPER_H__
#define __SQL_FIELD_WRAPPER_H__

#include <cjson_field.h>
#include <db_field_wrapper.h>
#include <string>
#include <map>
#include <vector>
#include <sql_driver.h>

#define FIELD_ID      "ID"
#define FIELD_POINTER    "INTEGER"
#define FIELD_CLASS_DATA "BLOB"
#define FIELD_CLASS_HASH "VARCHAR_HASH"
#define FIELD_INT     "INTEGER"
#define FIELD_STRING  "VARCHAR"
#define FIELD_POINTER "INTEGER"

#define TYPE_FIELD_COLUMN "colu_"
#define TYPE_FIELD_HASH   "hash_"
#define TYPE_FIELD_DATA	 "data_"

namespace dbcjson {

	void cmp(const dbcjson_dm_desc_t& aBase, const dbcjson_dm_desc_t& aComp, dbcjson_dm_desc_t& iInsert, dbcjson_dm_desc_t& iDelete);

	class sql_field_wrapper : public db_field_wrapper {

		public:

			sql_field_wrapper(cjson::field::field* iField) : db_field_wrapper(iField) {}
		
			virtual void sql_meta_description(const sql_driver* iDriver, dbcjson_dm_desc_t& ioDM){
				abort();
			}

			virtual void sql_create(const sql_driver* iDriver, dbcjson_dm_desc_t& ioDescr, std::vector<std::string> &ioSQL){
				abort(); // can't be saved
			}

			virtual void sql_insert_stmt(const db_driver* iDriver, std::string &ioSQL){
				abort(); // can't be saved
			}

			virtual void store(db_driver* iDriver, void* iDataPointer){
				abort();
			}

			virtual void sql_field(std::map<std::string,std::string> &ioSQL) = 0;

			virtual void sql_bind(sql_statement* iStatement, void* iDataPointer) = 0;

			virtual void prepare(db_driver* iDriver) {
				abort();
			};

			virtual bool isPrepared() {return false;}	
	
	};	


} 


#endif
