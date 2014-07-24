/*
 * Copyright 2013 BOUVIER-VOLAILLE Julien
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

#include <stdio.h>
#include <http_properties.h>
#include <http_server.h>
#include <http_secured_server.h>
#include <service_loader.h>

int main (void) {
  
	webcjson::http_properties::setPropertyFile("./etc/conf.properties");
	
	std::string aPortStr = webcjson::http_properties::getInstance().get("http_port");

	webcjson::http_server aServer(aPortStr);
	webcjson::http_secured_server aSecuredServer("5556");

	webcjson::service_loader::getInstance().loadServices(); 

	aServer.start();
	aSecuredServer.run();

}
