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
#ifndef __RPC_MODULE_H__
#define __RPC_MODULE_H__

#include <http_grammar.h>

namespace webcjson {

	class rpc_module {

	};
	
	template<typename CONTEXT_SESSION>
	class rpc_session_module {

		public:

			CONTEXT_SESSION* getContext(){
				return _session;
			}

			void initSession(){
				_session = new CONTEXT_SESSION();
			}

			void setContext(const std::string& iContext){
				_session = http_grammar::getInstance().getDictionary()->fromJson<CONTEXT_SESSION>(iContext);
			}				

			void extractContext(std::string& ioContext){
				ioContext = http_grammar::getInstance().getDictionary()->toJson<CONTEXT_SESSION>(_session);
			}

			void reset(){
				delete _session;
				_session = 0;
			}

		private:

			CONTEXT_SESSION* _session;			

	};	

}

#endif
