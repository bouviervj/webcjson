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
#ifndef __MYSQL_DRIVER_H__
#define __MYSQL_DRIVER_H__

#include <sql_driver.h>
#include <sql_statement.h>
#include <map>
#include "sql_field_wrapper.h"
#include <mysql.h>

namespace dbcjson {

	class mysql_driver : public sql_driver {

		public:

			mysql_driver(conf::Properties* iProperties) : sql_driver(iProperties) {

				_refs[FIELD_ID  ]    = "INT AUTO_INCREMENT PRIMARY KEY NOT NULL";
				_refs[FIELD_CLASS_HASH  ] = "VARCHAR(128)";
				_refs[FIELD_CLASS_DATA  ] = "BLOB";
				_refs[FIELD_INT    ] = "INTEGER";
				_refs[FIELD_STRING ] = "VARCHAR(767)";
				_refs[FIELD_POINTER] = "INTEGER";

			}

			mysql_driver() : sql_driver(NULL) {}

			~mysql_driver(){}

			virtual void connect();

			virtual bool isConnected();

			virtual void prepare();

			virtual bool isPrepared();
		
			virtual int execute(const std::string& iSQL);

			virtual sql_statement* getStatement(const std::string& iSQL);

			MYSQL *getMysqlConnection() const {
				return _con;
			}

			// Format "[CREATE DECL] %s (%s);"
			void SQL_CREATE_STR(std::string& ioCreate) const{
				ioCreate = "CREATE TABLE IF NOT EXISTS %s (%s);" ; 
			}

			void SQL_UNICITY_STR(std::string& ioUnicity) const {
				ioUnicity = "ALTER IGNORE TABLE %s ADD UNIQUE KEY %s(%s);";
			}

		     void SQL_DROP_UNICITY_STR(std::string& ioDrop) const {
				ioDrop = "ALTER TABLE %s DROP KEY %s";
			}	

			void SQL_ALTER_ADD_COLUMN_STR(std::string& ioAlter) const {
				ioAlter = "ALTER TABLE %s ADD COLUMN (%s);" ;
			}	

			void SQL_ALTER_DRP_COLUMN_STR(std::string& ioAlter) const {
				ioAlter = "ALTER TABLE %s DROP COLUMN %s;" ;
			}

			void SQL_INSERT_STR(std::string& ioInsert) const{
				ioInsert = "INSERT INTO %s (%s) VALUES (%s);";
			}

			void SQL_CONVERT_FIELD(const std::string& iType, std::string& ioCType) const {

				std::map<std::string,std::string>::const_iterator it = _refs.find(iType);
				if (it!=_refs.end()) {
					ioCType=it->second;
				} else {
					ioCType = "";
				}
			}

			virtual void getDescription(const std::string& ioType, dbcjson::dbcjson_dm_desc_t& iDescr) const;

			virtual void setDescription(const std::string& ioType, const dbcjson::dbcjson_dm_desc_t& iDescr) const;

		private:

			void executeAsStatement(const std::string ioSQL, MYSQL_BIND* iBind ) const;

			std::map<std::string, std::string> _refs;
			bool _connected;
			bool _prepared;
			MYSQL *_con;
	
	};	


} 


#endif
