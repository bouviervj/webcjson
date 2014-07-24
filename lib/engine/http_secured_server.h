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
#ifndef __HTTP_SECURED_SERVER_H__
#define __HTTP_SECURED_SERVER_H__

#include "http_server.h"

#include "openssl/ssl.h"

namespace webcjson {

	class http_secured_server : public http_server {

		public:
	
			http_secured_server(const std::string iPort) : http_server(iPort) {

				initHttpsParserSettings();
			
			}

			virtual int run (void);

		protected:

			int recordParserSocket(int iSock, SSL* iSSL, const std::string& iIP);

			virtual int read_from_client(int filedes);

			static int on_message_complete_https_server(http_parser* iParser);

			int initHttpsParserSettings();

			static int print_SSL_error(int iSSLError, bool read = true); 

		private:

			void set_nonblock(int socket);

			SSL_CTX* InitServerCTX(void);

			void LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile);

			void  ShowCerts(SSL* ssl);

			void  Servlet(SSL* ssl);

			

	};

}

#endif
