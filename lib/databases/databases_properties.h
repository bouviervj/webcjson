#ifndef __DATABASES_PROPERTIES_H__
#define __DATABASES_PROPERTIES_H__

#include <Properties.h>
#include <string>

namespace webcjson {

	class databases_properties {

		public : 

			static std::string _filename;

			static void setPropertyFile(const std::string& iFileName);

			static databases_properties& getInstance();
			
			const std::string& get(const std::string& iKey);
	
		private: 

			databases_properties(const std::string& iFileName);

			databases_properties(databases_properties const&);         
       		void operator=(databases_properties const&); 

			conf::Properties* _properties;


	};

} 

#endif
