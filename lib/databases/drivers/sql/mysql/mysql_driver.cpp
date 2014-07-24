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

#include "mysql_driver.h"

#include <string>
#include <string.h>
#include "mysql_statement.h"
#include <demangle.h>

// TODO change this inclusion
#include <databases_grammar.h>

namespace dbcjson {

	void mysql_driver::connect(){

		  _con = mysql_init(NULL);
		
		  std::string aConnectStr = (*_prop)["db.sql.mysql.connect"];
		  int aPort = atoi((*_prop)["db.sql.mysql.port"].c_str());	
		  std::string aUser = (*_prop)["db.sql.mysql.user"];
		  std::string aPassword = (*_prop)["db.sql.mysql.pass"];
		  std::string aSchema = (*_prop)["db.sql.mysql.schema"];
		 
		  _connected = false;

		  if (_con == NULL) 
		  {
			 fprintf(stderr, "%s\n", mysql_error(_con));
			 exit(1);
		  }

		  if (mysql_real_connect(_con, aConnectStr.c_str(), aUser.c_str(), aPassword.c_str(), 
				aSchema.c_str() , aPort, NULL, 0) == NULL) 
		  {
			 fprintf(stderr, "%s\n", mysql_error(_con));
			 mysql_close(_con);
		  } else {
			 fprintf(stdout, "Mysql connection established %s port: %d user: %s\n", aConnectStr.c_str(), aPort, aUser.c_str());
			 _connected = true;
		  }

		  mysql_set_server_option( _con , MYSQL_OPTION_MULTI_STATEMENTS_ON);

		  execute("set session old_alter_table=1"); 
	}

	bool mysql_driver::isConnected(){
		return _connected;
	}

	void mysql_driver::prepare(){
		
		_prepared = false;
		if (execute("CREATE TABLE IF NOT EXISTS webcjson_table_descr (name VARCHAR(767), description TEXT, PRIMARY KEY (name) ) ")) {
			_prepared  = true;
		}

	}

	bool mysql_driver::isPrepared(){
		return _prepared;
	}

     int mysql_driver::execute(const std::string& iSQL){

		if (!isConnected()) {
			connect();
		}

		if (isConnected()) {

			fprintf(stdout, "MYSQL EXECUTE : %s \n", iSQL.c_str());
			if (mysql_query(_con, iSQL.c_str())) {
				fprintf(stderr, "%s\n", mysql_error(_con));
				return 0;
			}

		} else {
			fprintf(stdout, "Mysql connection not established \n");
			return 0;
		}

		return 1;

	}

	sql_statement* mysql_driver::getStatement(const std::string& iSQL){

		return new mysql_statement((sql_driver*) this, iSQL);

	}

	void mysql_driver::getDescription(const std::string& ioType, dbcjson::dbcjson_dm_desc_t& iDescr) const {

		MYSQL_STMT    *stmt;
		MYSQL_BIND    bind[1];
		MYSQL_BIND    bind_result[1];
		
		stmt = mysql_stmt_init(_con);
		if (!stmt)
		{
		  fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		  exit(0);
		}

		char* aQuery = (char*) "SELECT description FROM webcjson_table_descr where name = ?";
		if (mysql_stmt_prepare(stmt, aQuery, strlen(aQuery)))
		{
		  fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}

		memset(bind, 0, sizeof(bind));

		const char* aStr = ioType.c_str();
		long unsigned int aSize = ioType.size();

		bind[0].buffer_type= MYSQL_TYPE_STRING;
		bind[0].buffer= (char *) aStr;
		bind[0].buffer_length= 2048;
		bind[0].is_null= 0;
		bind[0].length= &aSize;

		/* Bind the buffers */
		if (mysql_stmt_bind_param(stmt, bind))
		{
		  fprintf(stderr, " mysql_stmt_bind_param() failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}

		MYSQL_RES* aRes = mysql_stmt_result_metadata(stmt);

		my_bool aBool = 1;
		mysql_stmt_attr_set(stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &aBool);

		/* Execute the select statement - 1*/
		if (mysql_stmt_execute(stmt))
		{
		  fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}

		if (mysql_stmt_store_result(stmt)) {
		  fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}		

		MYSQL_FIELD* aField = &aRes->fields[0];
		
		fprintf(stdout, " field %s \n",aField->name);
		fprintf(stdout, " field length %d \n",(int) aField->length);
		fprintf(stdout, " field max length %d \n", (int) aField->max_length);
		
		
		int totalrows = mysql_stmt_num_rows(stmt);
		fprintf(stdout, " fetched %d description\n",totalrows);
		fprintf(stdout, " field count %d \n",(int) aRes->field_count);

		long unsigned int aMaxSize;
		char* aBuffer = (char*) malloc(aField->max_length);

		memset (bind_result, 0, sizeof (bind_result));
		bind_result[0].buffer_type= MYSQL_TYPE_BLOB;
		bind_result[0].is_null= 0;
		bind_result[0].buffer= (char *) aBuffer;
		bind_result[0].buffer_length= aField->max_length;
		bind_result[0].length= &aMaxSize;

		mysql_stmt_bind_result(stmt, bind_result);

		std::string JsonDescription;
		while(!mysql_stmt_fetch(stmt))
		{
			fprintf(stdout, " size %d\n", (int) aMaxSize);
			JsonDescription = std::string(aBuffer,aMaxSize);
			fprintf(stdout, " description %s\n", JsonDescription.c_str());

			dbcjson::dbcjson_dm_desc_t* aDescr =  webcjson::databases_grammar::getInstance().getDictionary()->fromJson< dbcjson::dbcjson_dm_desc_t>(JsonDescription);

			if (aDescr!=0) {
				iDescr = *aDescr;
			}

		}

		free(aBuffer);

		mysql_free_result(aRes);

		if (mysql_stmt_close(stmt))
		{
		  fprintf(stderr, " failed while closing the statement\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}

	}

	void mysql_driver::setDescription(const std::string& ioType, const dbcjson::dbcjson_dm_desc_t& iDescr) const{

		std::string aJsonDescription;

		char buffer[2048];
		demangle(buffer, 2048, typeid(iDescr).name());
		fprintf(stdout, "Description type: %s \n", buffer );

		std::string aJson = webcjson::databases_grammar::getInstance().getDictionary()->toJson< dbcjson::dbcjson_dm_desc_t>(&iDescr);
		fprintf(stdout, "Data: %s \n", aJson.c_str() );

		MYSQL_BIND    bind[2];
		char* aName = (char*)ioType.c_str();
		unsigned long int aSize = ioType.size();
		memset(bind, 0, sizeof(bind));

		bind[0].buffer_type= MYSQL_TYPE_STRING;
		bind[0].buffer= (char *) aName;
		bind[0].buffer_length= 2048;
		bind[0].is_null= 0;
		bind[0].length= &aSize;

		const char* aJsonStr = aJson.c_str();
		long unsigned int aJsonSize = aJson.size();

		bind[1].buffer_type= MYSQL_TYPE_BLOB;
		bind[1].buffer= (char *) aJsonStr;
		bind[1].buffer_length= aJsonSize;
		bind[1].is_null= 0;
		bind[1].length= &aJsonSize;

		executeAsStatement("DELETE FROM webcjson_table_descr where name = ?;", bind );	
		executeAsStatement("INSERT INTO webcjson_table_descr (name,description) VALUES (?,?);", bind );

	}

	void mysql_driver::executeAsStatement(const std::string ioSQL, MYSQL_BIND* iBind ) const{

		MYSQL_STMT  *stmt;
		
		stmt = mysql_stmt_init(_con);
		if (!stmt)
		{
		  fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		  exit(0);
		}

		char* aQuery = (char*) ioSQL.c_str();
		if (mysql_stmt_prepare(stmt, aQuery, strlen(aQuery)))
		{
		  fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}

		/* Bind the buffers */
		if (mysql_stmt_bind_param(stmt, iBind))
		{
		  fprintf(stderr, " mysql_stmt_bind_param() failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}

		/* Execute the select statement - 1*/
		if (mysql_stmt_execute(stmt))
		{
		  fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}

		if (mysql_stmt_close(stmt))
		{
		  fprintf(stderr, " failed while closing the statement\n");
		  fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		  exit(0);
		}

	}

}

