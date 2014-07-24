#include <service_module_fibo.h>
#include <http_grammar.h>
#include <jsonxx.h>
#include <string>

namespace webcjson {

	CREATE_RECORDED_SERVICE(service_module_fibo,"/module_fibo")

	int service_module_fibo::execute(const std::string& iFunction, const std::vector<std::string>& iArgs, std::string& ioResult){

		std::cout << " Execute service_module_fibo method." << std::endl ; 

		if (iFunction=="fibonacci") {

			int* aArg0=0;

			try {

				aArg0=http_grammar::getInstance().getDictionary()->fromJson<int>(iArgs[0]);

				int aReturn = _module.fibonacci(*aArg0);

				delete aArg0;

				ioResult = http_grammar::getInstance().getDictionary()->toJson<int>(&aReturn);

			} catch (jsonxx::jsonxx_exception const & ex) {

				//Building error stream
				std::ostringstream oStream;
				oStream << ex.what() << std::endl; 

				//Have to return bad request or body containing what()
				if (aArg0){
					delete aArg0;
				} else {
					oStream << "fibonacci: arg0 (int) not defined correctly received " << iArgs[0] << ".\n" << std::endl;
				}

				ioResult = oStream.str(); 

				return 1;
			}

		} else if (iFunction=="fibonacci_it") {

			int* aArg0=0;

			try {

				aArg0=http_grammar::getInstance().getDictionary()->fromJson<int>(iArgs[0]);

				int aReturn = _module.fibonacci_it(*aArg0);

				delete aArg0;

				ioResult = http_grammar::getInstance().getDictionary()->toJson<int>(&aReturn);

			} catch (jsonxx::jsonxx_exception const & ex) {

				//Building error stream
				std::ostringstream oStream;
				oStream << ex.what() << std::endl; 

				//Have to return bad request or body containing what()
				if (aArg0){
					delete aArg0;
				} else {
					oStream << "fibonacci_it: arg0 (int) not defined correctly received " << iArgs[0] << ".\n" << std::endl;
				}

				ioResult = oStream.str(); 

				return 1;
			}

		}

		return 0;

	}

	void service_module_fibo::returnDefinition(std::string& ioDefinition){
		ioDefinition="{\"fibonacci\":1,\"fibonacci_it\":1}";
	}

}

