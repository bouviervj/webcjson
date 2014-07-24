/*
 * Copyright 2013 BOUVIER-VOLAILLE Julien
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
#include "sql_wrapper_factory.h"

#include "sql_class_wrapper.h"
#include "./extended/stl/sql_std_string_wrapper.h"
#include "sql_fundamental_wrapper.h"

namespace dbcjson {

		SqlWrapperFactory::SqlWrapperFactory(){
			_dict = 0; // init
		}

		void SqlWrapperFactory::setDictionary(cjson::dictionary* iDict){
			_dict = iDict;
		}

		cjson::dictionary* SqlWrapperFactory::getDictionary(){
			return _dict;
		}

		void SqlWrapperFactory::init(cjson::dictionary* iDict){

			SqlWrapperFactory::getRegularInstance().setDictionary(iDict);

		}

		SqlWrapperFactory& SqlWrapperFactory::getRegularInstance(){
			static SqlWrapperFactory* _instance;
			if (!_instance) {
				_instance = new SqlWrapperFactory();
			}
			return *_instance;
		}

		SqlWrapperFactory& SqlWrapperFactory::getInstance(){

			// Here register factories
			CREATE_WRAPPER_FACTORY(sql_class_wrapper, cjson::field::class_field, CLASSWRAPPER)
			CREATE_WRAPPER_FACTORY(sql_generic_wrapper<cjson::field::fundamental_field<int> >, cjson::field::fundamental_field<int>, INTWRAPPER    ) 
			CREATE_WRAPPER_FACTORY(sql_std_string_wrapper , cjson::field::std_string_field, VARCHARWRAPPER)

			return getRegularInstance();
		} 


		sql_field_wrapper* SqlWrapperFactory::getSqlWrapper(const std::string& iFieldType) {

			// get field
			cjson::field::field* aField = _dict->getDataStruct(iFieldType);

			// extract field type
			char buffer[2048];
			demangle(buffer, 2048, typeid(*aField).name());
						     
			printf("requested type:%s\n",buffer);	

			GenericSqlWrapperFactory* aFact = _factories[std::string(buffer)];
			if (aFact) {
				return aFact->getSqlWrapper(iFieldType);
			}
#ifdef __DEBUG__
			printf("missing requested type:%s\n",iFieldType.c_str());	
#endif
			return NULL;
		}

		void SqlWrapperFactory::addFactory(const std::string& iFieldType, GenericSqlWrapperFactory* iFactory){
//#ifdef __DEBUG__
			printf("Registering wrapper factory: %s for [%p] from [%p]\n",iFieldType.c_str(),this,iFactory);
//#endif
			if (_factories.find(iFieldType)!=_factories.end()) {
				printf("Registering twice wrapper factory: %s for [%p] from [%p]\n",iFieldType.c_str(),this,iFactory);
				return;
			}

			_factories[iFieldType] = iFactory;
		}
		
		

}


