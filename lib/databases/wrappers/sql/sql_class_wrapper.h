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
#ifndef __SQL_CLASS_WRAPPER_H__
#define __SQL_CLASS_WRAPPER_H__

#include "sql_field_wrapper.h"

namespace dbcjson { 

	class sql_class_wrapper : public sql_field_wrapper {

		public:

			sql_class_wrapper(cjson::field::field* iField); 
		
			virtual void sql_meta_description(const sql_driver* iDriver, dbcjson_dm_desc_t& ioDM);

			virtual void sql_create(const sql_driver* iDriver, dbcjson_dm_desc_t& iDescr, std::vector<std::string> &ioSQL);

			virtual void sql_insert_stmt(const db_driver* iDriver, std::string &ioSQL);

			virtual void store(db_driver* iDriver, void* iDataPointer);

			virtual void sql_field(std::map<std::string,std::string> &ioSQL){

				ioSQL[TYPE_FIELD_DATA] = FIELD_CLASS_DATA;
				ioSQL[TYPE_FIELD_HASH] = FIELD_CLASS_HASH;
				//ioSQL = FIELD_CLASS;
			}	

			virtual void sql_bind(sql_statement* iStatement, void* iDataPointer);

			virtual void prepare(const db_driver* iDriver);

			virtual bool isPrepared();

		private:

			bool _prepared;
			std::string _id_name;	
			sql_statement* _createStatement;

	};	


} 


#endif
