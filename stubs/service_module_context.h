#ifndef _SERVICE_MODULE_CONTEXT_H__
#define _SERVICE_MODULE_CONTEXT_H__

#include <rpc_module.h>
#include <recorded_service.h>
#include <module_context.h>

namespace webcjson {

	class service_module_context : public module_session_service<module_context > { 

		public:

			service_module_context(const std::string& iURL) : module_session_service<module_context>(iURL) {
			}

			virtual int execute(const std::string& iFunction, const std::vector<std::string>& iArgs, std::string& ioResult);

			virtual void returnDefinition(std::string& ioDefinition);

			ADD_RECORDED_SERVICE(service_module_context);

	};

}

#endif
