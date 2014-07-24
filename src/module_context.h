#ifndef __MODULE_CONTEXT_H__
#define __MODULE_CONTEXT_H__

#include "../lib/engine/rpc_module.h"

	class context {

		public:
			int _count;
	
	};	

	class module_context : public webcjson::rpc_session_module<context> {

		public:	

			int inc() {
			   return getContext()->_count++; 
			}

	};



#endif 
