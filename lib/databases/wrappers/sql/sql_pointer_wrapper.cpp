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

#include "sql_pointer_wrapper.h"
#include "sql_wrapper_factory.h"

namespace dbcjson { 

	sql_pointer_wrapper::sql_pointer_wrapper(cjson::field::field* iField) : sql_field_wrapper(iField) {

	}
	
	void sql_pointer_wrapper::sql_bind(sql_statement* iStatement, void* iDataPointer){
		
		cjson::field::pointer_field* aPField = (cjson::field::pointer_field*) _innerField;
		cjson::field::field* aField = aPField->innerField();

		sql_field_wrapper* aWrap = SqlWrapperFactory::getInstance().getSqlWrapper(aField->getName());

		aWrap->store(iStatement->getDriver(), iDataPointer);

	}

}


