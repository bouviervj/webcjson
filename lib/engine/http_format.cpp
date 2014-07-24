/*
 * Copyright 2014 BOUVIER-VOLAILLE Julien
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
#include "http_format.h"


#include <string>
#include <iostream>
#include <sstream> 

#include "../utils/Properties.h"
#include "../utils/advanced_string.h"
#include "../utils/compress.h"

namespace webcjson {

#define HTTP_OK             200

#define HTTP_NOT_MODIFIED   304

#define HTTP_BAD_REQUEST    400
#define HTTP_UNAUTHORIZED   401
#define HTTP_NOT_FOUND      404

#define HTML_BAD_REQUEST   "<html><head><title>400 Bad Request</title></head><body>400 Bad Request</body></html>"
#define HTML_UNAUTHORIZED  "<html><head><title>404 Not Found</title></head><body>401 Unauthorized</body></html>"
#define HTML_NOT_FOUND     "<html><head><title>404 Not Found</title></head><body>404 Not Found</body></html>"

	std::map<int,std::string> _http_codes;
	std::map<int,std::string> _http_html;
	bool _http_codes_init = false;
	
	void initHttpCodes(){
		if (!_http_codes_init) {
			
			// 2XX
			_http_codes[HTTP_OK] = "OK";

			// 3XX
			_http_codes[HTTP_NOT_MODIFIED  ] = "NOT MODIFIED";

			// 4XX
			_http_codes[HTTP_BAD_REQUEST   ] = "BAD REQUEST";
			_http_codes[HTTP_UNAUTHORIZED  ] = "UNAUTHORIZED";
			_http_codes[HTTP_NOT_FOUND     ] = "NOT FOUND";

			_http_codes_init = true;

			// ############### HTML answer #################
			_http_html[HTTP_BAD_REQUEST]  = HTML_BAD_REQUEST;
			_http_html[HTTP_UNAUTHORIZED] = HTML_UNAUTHORIZED;
			_http_html[HTTP_NOT_FOUND]    = HTML_NOT_FOUND;


			 _http_codes_init = true;
		}
	}

	bool _content_types_init = false;
	conf::Properties* _content_types;

	void initContentType(){

		if (! _content_types_init) {

			_content_types = new conf::Properties("./etc/content_type.properties");				
	
			_content_types_init = true;

		}

	}

	std::string getContentType(const std::string& iExtension) {
		initContentType();
		return (*_content_types)[iExtension];
	}


	void buildBody(std::ostringstream& ioFormatted, http_context* const iContext, const std::string& iBody){

		// Here we proceed to compress data if we find accept encoding field;
		// TODO build map for conditionnal encoding not needed for some images for example
		
		std::string acceptEncoding("Accept-Encoding");
		std::string accept = iContext->_key_value[acceptEncoding];
		if (adv::contains(accept, "deflate" )) {
			std::ostringstream aCPData;
			cmp::zlib_compress_memory(iBody.c_str(), iBody.size(), aCPData);
			std::string aBody = aCPData.str();			
			ioFormatted << "Content-Length: " << aBody.size() << "\r\n";
			ioFormatted << "Content-Encoding: deflate" << "\r\n";
			ioFormatted << "\r\n";
			ioFormatted << aBody;
		} else {
			ioFormatted << "Content-Length: " << iBody.size() << "\r\n";
			ioFormatted << "\r\n";
			ioFormatted << iBody;
		}

	}

	std::string http_format(http_context* iContext){
		
		initHttpCodes();

		std::ostringstream formatted;
   
		formatted << "HTTP/1.1 ";
		formatted << iContext->_reply_code << " ";
		formatted << _http_codes[iContext->_reply_code] << "\r\n" ; // here search a map for the reply

		// Server name
		formatted << "Server: " << "Webcjson/0.0 Beta Version " << "\r\n";

		// add all header parameters
		typedef std::map<std::string, std::string>::iterator it_type;
		for(it_type iterator = iContext->_key_value_reply.begin(); iterator != iContext->_key_value_reply.end(); iterator++) {
		    // iterator->first = key
		    // iterator->second = value
		    formatted << iterator->first << ": " << iterator->second << "\r\n";
		}

		iContext->formatCookies(formatted);

		// here starting 
		if ((iContext->_reply_code<300) || (iContext->_reply_code>=400)) { // TODO enhance this
			
			if (iContext->_body_reply=="") {
				formatted << "Content-Type: " << getContentType("html") << ";" << "charset=utf-8" << "\r\n";
				std::string aReply = _http_html[iContext->_reply_code];
				buildBody( formatted, iContext, aReply );
			} else {		
				if (iContext->_extension_reply!="") {
				   formatted << "Content-Type: " << getContentType(iContext->_extension_reply) << ";" << "charset=utf-8" << "\r\n"; // TODO manage charset encoding
				}
				buildBody( formatted, iContext, iContext->_body_reply );
			}

		} else {
			formatted << "\r\n";
		}
	
    		return formatted.str();

	}


}
