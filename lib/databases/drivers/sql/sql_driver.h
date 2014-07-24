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
#ifndef __SQL_DRIVER_H__
#define __SQL_DRIVER_H__

#include <db_driver.h>
#include <db_field_wrapper.h>
#include <sql_statement.h>

namespace dbcjson {

	typedef std::map<std::string,std::string> dbcjson_dm_desc_t;

	class sql_driver : public db_driver {

		public:

			sql_driver() : db_driver(NULL) {}

			sql_driver(conf::Properties* iProperties) : db_driver(iProperties) {}

			~sql_driver(){}

			virtual int execute(const std::string& iSQL) = 0;

			virtual sql_statement* getStatement(const std::string& iSQL) = 0;

			// Format "[CREATE DECL] %s (%s);"
			virtual void SQL_CREATE_STR(std::string& ioCreate) const= 0;

			virtual void SQL_UNICITY_STR(std::string& ioCreate) const= 0;

			virtual void SQL_DROP_UNICITY_STR(std::string& ioDrop) const= 0;

			virtual void SQL_ALTER_ADD_COLUMN_STR(std::string& ioAlter) const= 0;

			virtual void SQL_ALTER_DRP_COLUMN_STR(std::string& ioAlter) const= 0;

			virtual void SQL_INSERT_STR(std::string& ioInsert) const= 0;

			virtual void SQL_CONVERT_FIELD(const std::string& iType, std::string& ioCType) const = 0;

			virtual void getDescription(const std::string& ioType, dbcjson::dbcjson_dm_desc_t& iDescr) const= 0;

			virtual void setDescription(const std::string& ioType, const dbcjson::dbcjson_dm_desc_t& iDescr) const= 0;
	
	};	


} 


#endif
