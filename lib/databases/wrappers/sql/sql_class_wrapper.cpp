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

#include "sql_class_wrapper.h"
#include "sql_wrapper_factory.h"
#include <cjson_class_field.h>

#include <iostream>
#include <sstream>

namespace dbcjson {

	sql_class_wrapper::sql_class_wrapper(cjson::field::field* iField) : sql_field_wrapper(iField) {


	}
		
	void sql_class_wrapper::sql_meta_description(const sql_driver* iDriver, dbcjson_dm_desc_t& ioDM){

		// TODO replace these 2 lines
		cjson::field::class_field* aCField = dynamic_cast<cjson::field::class_field*>(_innerField);
		std::map<std::string , cjson::field::attached_field* >& aList = aCField->getList();

		std::string aFieldType;

		_id_name = "id_"+aCField->getName();
		iDriver->SQL_CONVERT_FIELD(FIELD_ID, aFieldType);
		ioDM[_id_name] = aFieldType;

		typedef std::map<std::string , cjson::field::attached_field* >::iterator it_type;
		for(it_type iterator = aList.begin(); iterator != aList.end(); ) {

		    sql_field_wrapper* aWrap = SqlWrapperFactory::getInstance().getSqlWrapper(iterator->second->getField()->getName());	
		    std::map<std::string,std::string> iFieldColumnNames;
		    aWrap->sql_field(iFieldColumnNames); // This field common name have to be converted by the driver			

		    // for loop
		    std::map<std::string , std::string >::iterator it;
		    for( it = iFieldColumnNames.begin(); it != iFieldColumnNames.end(); ) {
		    		iDriver->SQL_CONVERT_FIELD(it->second, aFieldType);		
		    		ioDM[it->first+iterator->first] = aFieldType;	
				it++;
		    }

              iterator++;
		    //if (iterator!=aList.end()) aFormatted << "," ;
		}

	}

	
	void sql_class_wrapper::sql_create(const sql_driver* iDriver, dbcjson_dm_desc_t& iPreviousDescr, std::vector<std::string> &ioSQL){

		typedef dbcjson_dm_desc_t::iterator it_type;

		dbcjson_dm_desc_t aDescr;
		sql_meta_description(iDriver, aDescr);

		// TODO replace these 2 lines
		cjson::field::class_field* aCField = dynamic_cast<cjson::field::class_field*>(_innerField);
		std::string aTableName = "table_"+aCField->getName();
		std::string aIdName = "id_"+aCField->getName();
		std::string aUniqueName = "unique_"+aCField->getName();

		if (iPreviousDescr.size()==0) {  // CREATE TABLE IF NOT EXISTS i.e no description, create table

			if (_innerField) {	

				std::ostringstream aFormatted;
				std::ostringstream aUFormat;

				std::string prefix = TYPE_FIELD_DATA;
				std::vector<std::string> aFields;
				std::vector<std::string> aUFields;

				for(it_type iterator = aDescr.begin(); iterator != aDescr.end(); ) {
				    bool aUse = (aIdName!=iterator->first) && (iterator->first.substr(0, prefix.size()) != prefix);
				    aFields.push_back(iterator->first);
				    if (aUse) aUFields.push_back(iterator->first);
		              iterator++;
				}

				for (int i=0; i<aFields.size(); i++) {
					aFormatted << aFields[i] << " " << aDescr[ aFields[i]];
					if (i<aFields.size()-1){
					 aFormatted << "," ;
				    }
				}

				for (int i=0; i<aUFields.size(); i++) {
					aUFormat << aUFields[i];
					if (i<aUFields.size()-1){
					 aUFormat << "," ;
				    }
				}

				char buffer[2048];
				std::string aCS;

				// Create table
				iDriver->SQL_CREATE_STR(aCS);
				int res = snprintf(buffer, 2048, aCS.c_str(), aTableName.c_str(), aFormatted.str().c_str());
				ioSQL.push_back(std::string(buffer, res));


				// Add unicity constraint
				iDriver->SQL_UNICITY_STR(aCS);
				res = snprintf(buffer, 2048, aCS.c_str(), aTableName.c_str(), aUniqueName.c_str(), aUFormat.str().c_str());
				ioSQL.push_back(std::string(buffer, res));

				iPreviousDescr = aDescr; // TODO to be changed 
			
			}

		} else { // ALTER TABLE , add remove column

			
			dbcjson_dm_desc_t aInsertDescr;
			dbcjson_dm_desc_t aDeleteDescr;
			cmp(iPreviousDescr, aDescr, aInsertDescr, aDeleteDescr);

			char buffer[2048];
			std::string aCS;
			iDriver->SQL_DROP_UNICITY_STR(aCS);
			int res = snprintf(buffer, 2048, aCS.c_str(), aTableName.c_str(), aUniqueName.c_str());
			
			ioSQL.push_back(std::string(buffer, res));

			for(it_type iterator = aDeleteDescr.begin(); iterator != aDeleteDescr.end(); iterator++) {

				std::ostringstream aFormatted;

				aFormatted << iterator->first ;

				iDriver->SQL_ALTER_DRP_COLUMN_STR(aCS);
				res = snprintf(buffer, 2048, aCS.c_str(), aTableName.c_str(), aFormatted.str().c_str());
				ioSQL.push_back(std::string(buffer, res));
	
			}
			
			
			for(it_type iterator = aInsertDescr.begin(); iterator != aInsertDescr.end(); iterator++) {

				std::ostringstream aFormatted;

				aFormatted << iterator->first << " " << iterator->second;

				iDriver->SQL_ALTER_ADD_COLUMN_STR(aCS);
				int res = snprintf(buffer, 2048, aCS.c_str(), aTableName.c_str(), aFormatted.str().c_str());
				ioSQL.push_back(std::string(buffer, res));
	
			}

			std::ostringstream aUFormat;

			std::string prefix = TYPE_FIELD_DATA;
			std::vector<std::string> aFields;
	
			for(it_type iterator = aDescr.begin(); iterator != aDescr.end(); ) {
			    bool aUnicUse = (aIdName!=iterator->first) && (iterator->first.substr(0, prefix.size()) != prefix);
			    if (aUnicUse) aFields.push_back( iterator->first );
	              iterator++;
			}

			for (int i=0; i<aFields.size(); i++) {
			    aUFormat << aFields[i];
			    if (i<aFields.size()-1){
				 aUFormat << "," ;
			    }
			}

			// Add unicity constraint
			iDriver->SQL_UNICITY_STR(aCS);
			res = snprintf(buffer, 2048, aCS.c_str(), aTableName.c_str(), aUniqueName.c_str(), aUFormat.str().c_str());
			ioSQL.push_back(std::string(buffer, res));


			iPreviousDescr = aDescr;	
			
		}			


	}

	bool Sort_by_string(const std::string& a ,const std::string& b)
	{
		return a < b;
	}

	
	void sql_class_wrapper::sql_insert_stmt(const db_driver* iDriver, std::string &ioSQL){

		const sql_driver* aDriver =  (const sql_driver*) iDriver; 

		typedef dbcjson_dm_desc_t::iterator it_type;

		dbcjson_dm_desc_t aDescr;
		sql_meta_description(aDriver, aDescr);

		// TODO create a getName function
		cjson::field::class_field* aCField = dynamic_cast<cjson::field::class_field*>(_innerField);
		std::string aTableName = "table_"+aCField->getName();

		if (_innerField) {	
		
			std::ostringstream aFmtNames;
			std::ostringstream aParams;

			for(it_type iterator = aDescr.begin(); iterator != aDescr.end(); ) {

			    if (_id_name!=iterator->first) {
				    if (iterator!=aDescr.begin()) {
						aFmtNames << "," ;
						aParams << ",";
				    }
				    aFmtNames << iterator->first;	
				    aParams << "?";
			    }     
			    iterator++;
				
			}

			char buffer[2048];
			std::string aCS;
			aDriver->SQL_INSERT_STR(aCS);
			int res = snprintf(buffer, 2048, aCS.c_str(), aTableName.c_str(), aFmtNames.str().c_str(), aParams.str().c_str() );

			ioSQL = std::string(buffer, res);

			printf("insert query: %s\n",ioSQL.c_str());

		}

	}

	void sql_class_wrapper::prepare(const db_driver* iDriver){

		sql_driver* aDriver = (sql_driver*) iDriver;

		if (aDriver->isPrepared()) {

			// TODO replace these 2 lines
			cjson::field::class_field* aCField = dynamic_cast<cjson::field::class_field*>(_innerField);
			std::string aTableName = "table_"+aCField->getName();

			std::vector<std::string> aSQLs;
			dbcjson::dbcjson_dm_desc_t aDescr;
			aDriver->getDescription(aTableName, aDescr);

			sql_create(aDriver, aDescr , aSQLs);

			for (int i=0; i<aSQLs.size(); i++) {
				aDriver->execute(aSQLs[i]);
			}

			aDriver->setDescription(aTableName, aDescr);
	
			// Here create all statements

			std::string ioSQL;
			sql_insert_stmt(iDriver, ioSQL);
			_createStatement = aDriver->getStatement(ioSQL);

			printf( "%s\n", ioSQL.c_str());

			_prepared = true;

		}

	}

	bool sql_class_wrapper::isPrepared(){
		return _prepared;
	}

	void sql_class_wrapper::store(db_driver* iDriver, void* iDataPointer){

		sql_driver* aDriver = (sql_driver*) iDriver;

		iDriver->prepare();

		prepare(iDriver);

		if (isPrepared()) { // Here the table has been created

		   // TODO replace these 2 lines
		   cjson::field::class_field* aCField = dynamic_cast<cjson::field::class_field*>(_innerField);
		   std::map<std::string , cjson::field::attached_field* >& aList = aCField->getList();

		   _createStatement->reset();	

		   dbcjson_dm_desc_t aDescr;
		   sql_meta_description(aDriver, aDescr); 	

		   typedef dbcjson_dm_desc_t::iterator it_type;
		   std::string iFieldName;

		   std::string prefix = TYPE_FIELD_DATA;

		   for(it_type iterator = aDescr.begin(); iterator != aDescr.end(); ) {
   
				
				
				if ((iterator->first!=_id_name) && (iterator->first.substr(0, prefix.size()) != prefix)) {

					iFieldName = iterator->first.substr(5);

					printf("try field: %s \n",iFieldName.c_str()) ;
					cjson::field::attached_field* aField = aList[iFieldName];
					if (aField) {

						void* aFieldAddress = (void*) ((unsigned long) iDataPointer+aField->getOffset()/8); 				
						sql_field_wrapper* aWrap = SqlWrapperFactory::getInstance().getSqlWrapper(aField->getField()->getName());

						_createStatement->bind(aWrap, aFieldAddress);

					}

				}

				iterator++;

		    }

		    _createStatement->execute(); 

		}
	
	}

	// used to store a class itself
	void sql_class_wrapper::sql_bind(sql_statement* iStatement, void* iDataPointer){
	
		std::ostringstream aStream;
		_innerField->toJson(aStream, iDataPointer);
		std::string aJson = aStream.str();

		iStatement->bindBLOB(aJson.c_str(), aJson.size());

	}


}
