#ifndef __HTTP_PROPERTIES_H__
#define __HTTP_PROPERTIES_H__

#include <Properties.h>
#include <string>

namespace webcjson {

	class http_properties {

		public : 

			static std::string _filename;

			static void setPropertyFile(const std::string& iFileName);

			static http_properties& getInstance();
			
			const std::string& get(const std::string& iKey);
	
		private: 

			http_properties(const std::string& iFileName);

			http_properties(http_properties const&);         
       		void operator=(http_properties const&); 

			conf::Properties* _properties;


	};

} 

#endif
