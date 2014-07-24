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
#ifndef __CJSON_SMART_POINTER__
#define __CJSON_SMART_POINTER__


namespace cjson {


	// this is a simplified version of smart pointer
	// not shared pointer with counter etc ... simply releasing data	
	template<typename TYPE>
	class smart_ptr {

		private:

			TYPE* _data;
			void(*release)(TYPE*);

		public:
			smart_ptr(TYPE* iData, void (*iRelease) (TYPE*) = NULL){
				_data = iData;
				release = iRelease;
			}

			~smart_ptr(){
				if (release!=NULL) {
					(*release)(_data);
				} else {
					free(_data);
				}
			}

			TYPE* get(){
				return _data;	
			}

	};

}


#endif // __CJSON_SMART_POINTER__
