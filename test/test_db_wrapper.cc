
#include <cjson_dictionary.h>
#include <sql_wrapper_factory.h>
#include <mysql_driver.h>
#include <databases_properties.h>

#include "../src/market_item.h"

using namespace std;
using namespace cjson;

int main(int argc, char **argv)
{

	conf::Properties* _properties = new conf::Properties("./etc/conf.properties");

	webcjson::databases_properties::setPropertyFile("./etc/conf.properties");

	// dictionary initialization
	cjson::dictionary aDict("./etc/grammar.xml");

	dbcjson::SqlWrapperFactory::init(&aDict);

	dbcjson::mysql_driver aDriver(_properties);

	aDriver.connect();
	
	dbcjson::sql_field_wrapper* aWrapper = dbcjson::SqlWrapperFactory::getInstance().getSqlWrapper("market_item");

	market_item aItem;
	aItem._name = "JULIEN";
	aItem._price = 210;
	aItem._type = 2;
	aItem._color = "RED";

	aWrapper->store(&aDriver, &aItem);

	free(aWrapper);

	
	
	return 0;

}


