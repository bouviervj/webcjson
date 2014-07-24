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
#include "mysql_statement.h"
#include "string.h"
#include <md5.h>

namespace dbcjson {

	void mysql_statement::init(sql_driver* iDriver){

		const mysql_driver* aDriver = (mysql_driver*) iDriver; 
		MYSQL *aCon = aDriver->getMysqlConnection();
		_stmt = mysql_stmt_init(aCon);

		if (!_stmt) {
		    fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		    return;
		}

		if (mysql_stmt_prepare(_stmt, _sql.c_str(), strlen(_sql.c_str()))) {
		    fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		    fprintf(stderr, " %s\n", mysql_stmt_error(_stmt));
		    return;
		}

		_param_count= mysql_stmt_param_count(_stmt); // get number of items

		_bind = new MYSQL_BIND[_param_count];

		reset();

	}

	int mysql_statement::execute(){

		/* Bind the buffers */
		if (mysql_stmt_bind_param(_stmt, _bind))
		{
		  fprintf(stderr, " mysql_stmt_bind_param() failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(_stmt));
		  exit(0);
		}

		if (mysql_stmt_execute(_stmt))
		{
		  fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(_stmt));
		  exit(0);
		}
		
	}

	void mysql_statement::reset(){

		_param_recorded = 0;
		memset(_bind, 0, sizeof(MYSQL_BIND)*_param_count);

		for (int i=0; i<_toBeFree.size(); i++) {
			free( _toBeFree[i] );
		}		

	}	

#define STRING_SIZE 1024

	void mysql_statement::bindBLOB(const void* iData, size_t iSize){

		printf("Bind mysql blb ...\n");

		if (_param_recorded+1>=_param_count) { abort(); }	

		void* aData = (void*) malloc(iSize);
		memcpy(aData,iData, iSize);
		_toBeFree.push_back(aData);

		_bind[_param_recorded].buffer_type= MYSQL_TYPE_BLOB;
		_bind[_param_recorded].buffer= (char *) aData;
		_bind[_param_recorded].buffer_length= iSize;
		_bind[_param_recorded].is_null= 0;
		_bind[_param_recorded].length= 0;

		_param_recorded++;

		MD5 md5;

		std::string aHash = md5(iData,iSize);

		void* aDataHash = (void*) malloc(aHash.size());
		memcpy(aDataHash,aHash.c_str(),aHash.size());
		_toBeFree.push_back(aDataHash);

		unsigned long* aLength = (unsigned long*) malloc(sizeof(unsigned long));
		*aLength = aHash.size();
		_toBeFree.push_back(aLength);

		_bind[_param_recorded].buffer_type= MYSQL_TYPE_STRING;
		_bind[_param_recorded].buffer= aDataHash;
		_bind[_param_recorded].buffer_length= STRING_SIZE;
		_bind[_param_recorded].is_null= 0;
		_bind[_param_recorded].length= aLength; 

		_param_recorded++;

	}

	void mysql_statement::bindINTEGER(int iInteger){
		printf("Bind mysql integer ...\n");

		if (_param_recorded>=_param_count) { abort(); }	

		unsigned long* aInteger = (unsigned long*) malloc(sizeof(unsigned long));
		*aInteger = iInteger;
		_toBeFree.push_back(aInteger);

		_bind[_param_recorded].buffer_type= MYSQL_TYPE_LONG;
		_bind[_param_recorded].buffer= (char *) aInteger;
		_bind[_param_recorded].is_null= 0;
		_bind[_param_recorded].length= 0;

		_param_recorded++;

	}



	void mysql_statement::bindVARCHAR(const char* iData, size_t iSize){

		printf("Bind mysql varchar (text) ...\n");
		printf("%s (%d)...\n",iData,(int) iSize);
		
		if (_param_recorded>=_param_count) { abort(); }	

		char* aData = (char*) malloc(strlen(iData));
		memcpy(aData,iData,strlen(iData));
		_toBeFree.push_back(aData);

		unsigned long* aLength = (unsigned long*) malloc(sizeof(unsigned long));
		*aLength = iSize;
		_toBeFree.push_back(aLength);

		_bind[_param_recorded].buffer_type= MYSQL_TYPE_STRING;
		_bind[_param_recorded].buffer= aData;
		_bind[_param_recorded].buffer_length= STRING_SIZE;
		_bind[_param_recorded].is_null= 0;
		_bind[_param_recorded].length= aLength; 

		_param_recorded++;

	}


}
