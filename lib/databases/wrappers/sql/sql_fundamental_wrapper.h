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
#ifndef __SQL_FUNDAMENTAL_WRAPPER_H__
#define __SQL_FUNDAMENTAL_WRAPPER_H__

#include "sql_field_wrapper.h"
#include <cjson_fundamental_field.h>

namespace dbcjson { 

	template<typename T>
	class sql_generic_wrapper : public sql_field_wrapper {

		public:

			sql_generic_wrapper(cjson::field::field* iField): sql_field_wrapper(iField) {

			} 
		
			virtual void sql_field(std::map<std::string, std::string> &ioSQL) ;

			virtual void sql_bind(sql_statement* iStatement, void* iDataPointer);

	};	

#define FUNDAMENTAL_WRAPPER(TYPE,FIELD_DEF) \
template<> \
void sql_generic_wrapper<cjson::field::fundamental_field<TYPE> >::sql_field(std::map<std::string,std::string> &ioSQL){\
	ioSQL[TYPE_FIELD_COLUMN] = FIELD_DEF;					   		 \
}									  	

FUNDAMENTAL_WRAPPER(int    		,FIELD_INT   )

template<>
void sql_generic_wrapper<cjson::field::fundamental_field<int> >::sql_bind(sql_statement* iStatement, void* iDataPointer){
	
	iStatement->bindINTEGER(*((int*)iDataPointer));

}

}

#endif
