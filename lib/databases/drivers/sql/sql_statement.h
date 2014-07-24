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
#ifndef __SQL_STATEMENT_H__
#define __SQL_STATEMENT_H__

#include <string>

/*
#define FIELD_ID      "ID"
#define FIELD_CLASS   "BLOB"
#define FIELD_INT     "INTEGER"
#define FIELD_STRING  "VARCHAR"
#define FIELD_POINTER "INTEGER"
*/

namespace dbcjson {

	class sql_driver;
	class sql_field_wrapper;

	class sql_statement  {

		public:

			sql_statement(sql_driver* iDriver, const std::string& ioSQL){

				_driver = iDriver;
	
			}

			~sql_statement(){}

			sql_driver* getDriver(){
				return _driver;
			}

			virtual int execute() = 0;

			virtual void reset() = 0;

			virtual void bind(sql_field_wrapper* iWrapper, void* iPointer);

			virtual void bindBLOB(const void* iData, size_t iSize) = 0;

		     virtual void bindINTEGER(int iInteger) = 0;

			virtual void bindVARCHAR(const char* iData, size_t iSize) = 0;

		public:

			sql_driver* _driver;

	};	


} 


#endif
