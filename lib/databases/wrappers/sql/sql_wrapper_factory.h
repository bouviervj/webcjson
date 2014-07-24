
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
#ifndef __DB_WRAPPER_FACTORY_H__
#define __DB_WRAPPER_FACTORY_H__

#include <sql_field_wrapper.h>
#include <cjson_dictionary.h>
#include "demangle.h"
#include <typeinfo>

namespace dbcjson {

	class GenericSqlWrapperFactory {

		public :
			virtual sql_field_wrapper* getSqlWrapper(const std::string& iFieldType) = 0;
		
	};

	class SqlWrapperFactory {

		public :		

			static void init(cjson::dictionary* iDict);	

			static SqlWrapperFactory& getInstance();
	
			static SqlWrapperFactory& getRegularInstance();

			void setDictionary(cjson::dictionary* iDict);

			cjson::dictionary* getDictionary();
			
			sql_field_wrapper* getSqlWrapper(const std::string& iFieldType);

			void addFactory(const std::string& iWrapperType, GenericSqlWrapperFactory* iFactory);
	
		private : 		
	
			SqlWrapperFactory();

			std::map<std::string,GenericSqlWrapperFactory*> _factories;
			cjson::dictionary* _dict;

		public :
			static SqlWrapperFactory* _instance;

	};


	// T wrapper type, U Wrapped type
	template<typename T, typename U> 
	class SqlFieldWrapperFactory : GenericSqlWrapperFactory {

		private :

			SqlFieldWrapperFactory(){
				char buffer[2048];
				demangle(buffer, 2048, typeid(U).name());
				SqlWrapperFactory::getRegularInstance().addFactory( std::string(buffer) , this);
			}

		public :	

			
			SqlFieldWrapperFactory<T,U>& getInstance();

			virtual sql_field_wrapper* getSqlWrapper(const std::string& iFieldType) {
				cjson::dictionary* aDict = SqlWrapperFactory::getRegularInstance().getDictionary();
				cjson::field::field* aField =  aDict->getDataStruct(iFieldType);
				return new T(aField);
			}

			friend class SqlWrapperFactory;

	};	

	#define CREATE_WRAPPER_FACTORY(TYPE,ASSOC,NAME) static dbcjson::SqlFieldWrapperFactory<TYPE,ASSOC>* NAME##_instance = new dbcjson::SqlFieldWrapperFactory<TYPE,ASSOC>(); 

}

#endif
