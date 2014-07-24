#ifndef __MARKET_ITEM_H__
#define __MARKET_ITEM_H__

#include <string>

	class nested_item {
			std::string _name_nested;
			std::string _type_nested;
	};

	class market_item {

		public:

			std::string _name;
			int _price;
			int _type;
			std::string _color;	
		
			nested_item _nest;		

	};	


#endif 
