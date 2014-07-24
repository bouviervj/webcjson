#ifndef _SERVICE_MODULE_FIBO_H__
#define _SERVICE_MODULE_FIBO_H__

#include <rpc_module.h>
#include <recorded_service.h>
#include <module_fibo.h>

namespace webcjson {

	class service_module_fibo : public module_service<module_fibo > { 

		public:

			service_module_fibo(const std::string& iURL) : module_service<module_fibo>(iURL) {
			}

			virtual int execute(const std::string& iFunction, const std::vector<std::string>& iArgs, std::string& ioResult);

			virtual void returnDefinition(std::string& ioDefinition);

			ADD_RECORDED_SERVICE(service_module_fibo);

	};

}

#endif
