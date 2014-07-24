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
#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <http_parser.h>
#include <http_context.h>
#include <http_format.h>
#include <engine.h>

#include <map>
#include <string>

#include "./lib/utils/Properties.h"

#define PORT    80    // Default http port
#define MAXMSG  512

namespace webcjson {

	class http_server {

		public:

			http_server(const std::string& iPort);

			virtual int run (void);

			pthread_t& start();
	
			static void* staticThread(void *arg);

			engine& getEngine(){
				return *_engine;
			}

		protected: 

			// PARSER CALLBACKS
			static int on_message_begin_server(http_parser* iParser);
			static int on_headers_complete_server(http_parser* iParser);
			static int on_message_complete_server(http_parser* iParser);
			static int on_url_server(http_parser* iParser, const char *at, size_t length);
			static int on_header_field_server(http_parser* iParser, const char *at, size_t length);
			static int on_header_value_server(http_parser* iParser, const char *at, size_t length);
			static int on_body_server(http_parser* iParser, const char *at, size_t length);
			static int build_reply(http_context* aCtx, std::string& ioStr);

			// SERVER INIT
			int initEngine();
			int initHttpParserSettings();
			int recordParserSocket(int iSock, const std::string& iIP);
			int make_socket (uint16_t port);
			virtual int read_from_client (int filedes);			

		protected:

			std::string _portNumber;

			http_parser_settings settings;

			http_context _contexts[FD_SETSIZE];
			
			engine* _engine;

			int _port;

			pthread_t _thread;

	};


}

#endif

