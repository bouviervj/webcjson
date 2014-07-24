#include <service_module_context.h>
#include <http_grammar.h>
#include <jsonxx.h>
#include <string>

namespace webcjson {

	CREATE_RECORDED_SERVICE(service_module_context,"/module_context")

	int service_module_context::execute(const std::string& iFunction, const std::vector<std::string>& iArgs, std::string& ioResult){

		std::cout << " Execute service_module_context method." << std::endl ; 

		if (iFunction=="inc") {


			try {


				int aReturn = _module.inc();


				ioResult = http_grammar::getInstance().getDictionary()->toJson<int>(&aReturn);

			} catch (jsonxx::jsonxx_exception const & ex) {

				//Building error stream
				std::ostringstream oStream;
				oStream << ex.what() << std::endl; 

				//Have to return bad request or body containing what()

				ioResult = oStream.str(); 

				return 1;
			}

		}

		return 0;

	}

	void service_module_context::returnDefinition(std::string& ioDefinition){
		ioDefinition="{\"inc\":0}";
	}

}

