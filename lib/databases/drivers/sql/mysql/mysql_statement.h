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
#ifndef __MYSQL_STATEMENT_H__
#define __MYSQL_STATEMENT_H__

#include <sql_statement.h>
#include <mysql_driver.h>
#include <mysql.h>
#include <vector>

namespace dbcjson {

	class mysql_statement : public sql_statement {

		public:

			mysql_statement(sql_driver* iDriver, const std::string& ioSQL) : sql_statement(iDriver, ioSQL){
				_sql = ioSQL;
				init(iDriver);
			}

			~mysql_statement(){

				delete _bind;
			
			}
			
			virtual int execute();

			virtual void reset();

			virtual void bindBLOB(const void* iData, size_t iSize);

			virtual void bindINTEGER(int iInteger);

			virtual void bindVARCHAR(const char* iData, size_t iSize);

		private:

			std::string _sql;
			int _param_count;
			int _param_recorded;
			MYSQL_STMT* _stmt;
			MYSQL_BIND* _bind;

			std::vector<void*> _toBeFree; // !

			void init(sql_driver* iDriver);
	
	};	


} 


#endif
