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
#ifndef __HTTP_CONTEXT_H__
#define __HTTP_CONTEXT_H__

#include "../http-parser/http_parser.h"

#include <string>
#include <map>
#include "openssl/ssl.h"

namespace webcjson {
	class http_server;
}

struct http_cookie {

	//state
	bool _new;
	bool _remove;

	// attr
	std::string _name;
	std::string _value;
	time_t _expiration_time;
	std::string _path;
	std::string _domain;
	bool _secure;	
	bool _httpOnly;	

};

struct http_context {

	// Inner ctx
	webcjson::http_server* _server;
	http_parser _parser;

	// Secured
	bool _secured;

	// IP Client
	std::string _ip;

	// Sock
	int _sock;
	SSL* _ssl;
	bool _ssl_nego;	

	// Status
	int _connect;
	bool _has_build_reply;
	http_parser_url _parsed_url;
	int _parsed_url_result;

	// Query
	std::string _url;
	std::string _lastkey;
	std::map<std::string,std::string > _key_value;
	std::string _body;

	std::map<std::string,std::string > _params;

	// Reply
	int _reply_code;
	std::string _extension_reply;
	std::map<std::string,std::string > _key_value_reply;
	std::string _body_reply;	

	// Cookies
	std::map<std::string,http_cookie> _cookies;

	void addCookie(const std::string& iName, 
				const std::string& iValue, 
				const std::string& iDomain, 
				const std::string& iPath, 
				const long iExpires, 
				bool iSecure=false, 
				bool iHttpOnly=false);

	http_cookie* getCookie(const std::string& iKey);

	void removeCookie(const std::string& iName);

	void formatCookies(std::ostringstream& ioFormatted);
	
	void extractCookies();

	void deleteCookies();

	void dumpCookies();

};

#endif
