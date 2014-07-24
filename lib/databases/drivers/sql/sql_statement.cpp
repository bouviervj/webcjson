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

#include "sql_statement.h"

#include <sql_field_wrapper.h>

namespace dbcjson {

	void sql_statement::bind(sql_field_wrapper* iWrapper, void* iPointer){
		printf("Bind %p \n", iPointer);
		iWrapper->sql_bind(this, iPointer);	
	}

}
