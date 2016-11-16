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
#include "http_secured_server.h"

#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/err.h"
#include <fcntl.h>
#include <assert.h>

namespace webcjson {

#define FAIL    -1


	int http_secured_server::on_message_complete_https_server(http_parser* iParser){
		//fprintf (stdout,"MESSAGE COMPLETE\n");
		http_context* _context = (http_context*) iParser->data;

		std::string aReply;
		build_reply( _context , aReply);

		//printf("OUTPUT \"%s\"\n", aReply.c_str());	

		ssize_t result = SSL_write(_context->_ssl, aReply.c_str(), aReply.size()); 

		int aSSLError = SSL_get_error(_context->_ssl, result);
		
		print_SSL_error(aSSLError,false);
	
		return 0;
	}

	int http_secured_server::initHttpsParserSettings(){

		settings.on_message_complete = on_message_complete_https_server;

	}

	void http_secured_server::set_nonblock(int socket) {
	    int flags;
	    flags = fcntl(socket,F_GETFL,0);
	    assert(flags != -1);
	    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
	}

	SSL_CTX* http_secured_server::InitServerCTX(void)
	{   const SSL_METHOD *method;
	    SSL_CTX *ctx;

	    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
	    SSL_load_error_strings();   /* load all error messages */
	    method = TLSv1_2_server_method();  /* create new server-method instance */
	    ctx = SSL_CTX_new(method);   /* create new context from method */
	    if ( ctx == NULL )
	    {
		   ERR_print_errors_fp(stderr);
		   abort();
	    }
	    return ctx;
	}

	void http_secured_server::LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile)
	{
	    //New lines
 
	    if (SSL_CTX_load_verify_locations(ctx, CertFile, KeyFile) != 1)
		   ERR_print_errors_fp(stderr);

	    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
		   ERR_print_errors_fp(stderr);
	    //End new lines

	    /* set the local certificate from CertFile */
	    if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0)
	    {
		   ERR_print_errors_fp(stderr);
		   abort();
	    }
	    /* set the private key from KeyFile (may be the same as CertFile) */
	    if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0)
	    {
		   ERR_print_errors_fp(stderr);
		   abort();
	    }
	    /* verify private key */
	    if (!SSL_CTX_check_private_key(ctx))
	    {
		   fprintf(stderr, "Private key does not match the public certificate\n");
		   fprintf(stderr,"\n");
		   abort();
	    }

	    //New lines - Force the client-side have a certificate // TODO add it in setup
	    //SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
	    SSL_CTX_set_verify_depth(ctx, 4);
	    //End new lines
	}

	void  http_secured_server::ShowCerts(SSL* ssl)
	{   X509 *cert;
	    char *line;

	    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
	    if ( cert != NULL )
	    {
		   printf("Server certificates:\n");
		   line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		   printf("Subject: %s\n", line);
		   free(line);
		   line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		   printf("Issuer: %s\n", line);
		   free(line);
		   X509_free(cert);
	    }
	    else
		   printf("No certificates.\n");
	}

	int http_secured_server::recordParserSocket(int iSock, SSL* iSSL, const std::string& iIP){

		http_parser_init(&_contexts[iSock]._parser, HTTP_REQUEST);
		_contexts[iSock]._sock = iSock;
		_contexts[iSock]._server = this;
		_contexts[iSock]._parser.data = (void*) &_contexts[iSock];
		_contexts[iSock]._connect = 1;
		_contexts[iSock]._ssl = iSSL;
		_contexts[iSock]._ssl_nego = false;
		_contexts[iSock]._secured = true;
		_contexts[iSock]._ip = iIP;

		read_from_client(iSock);
		
	}

	int http_secured_server::print_SSL_error(int iSSLError, bool read ) {

		char* aRW = "read";
		if (!read) {
			aRW = "write";
		}

		switch(iSSLError) {
               case SSL_ERROR_WANT_READ:
                   printf( "SSL_ERROR_WANT_READ in SSL_%s()",aRW);
                   break;
               case SSL_ERROR_WANT_WRITE:
                   printf( "SSL_ERROR_WANT_WRITE in SSL_%s()",aRW);
                   break;
               case SSL_ERROR_ZERO_RETURN:
                   printf( "SSL_ERROR_ZERO_RETURN in SSL_%s()",aRW);
                   break;
               case SSL_ERROR_NONE:
                   printf( "SSL_ERROR_NONE in SSL_%s() ",aRW);
                   break;
               case SSL_ERROR_WANT_CONNECT:
                   printf( "SSL_ERROR_WANT_CONNECT in SSL_%s()",aRW);
                   break;
               case SSL_ERROR_WANT_X509_LOOKUP:
                   printf( "SSL_ERROR_WANT_X509_LOOKUP in SSL_%s()",aRW);
                   break;
               case SSL_ERROR_SYSCALL:
                   printf( "SSL_ERROR_SYSCALL in SSL_%s()",aRW);
                   break;
               case SSL_ERROR_SSL:
                   printf( "SSL_ERROR_SSL in SSL_%s()",aRW);
                   break;
               default:
                   printf( "default: ERROR in %s in ssl",aRW); 	
		}

		printf("\nssl_error: %d\n", iSSLError);

	}

	int http_secured_server::read_from_client(int filedes) /* Serve the connection -- threadable */
	{  
	    char buf[1024];
	    int bytes;

	    http_context* aCtx = &_contexts[filedes];		
	    SSL* ssl = aCtx->_ssl;

	    if (!aCtx->_ssl_nego) {

		    if ( SSL_accept(ssl) != FAIL )     /* do SSL-protocol accept */
			  
		    {
			   ShowCerts(ssl);        /* get any certificates */
			   aCtx->_ssl_nego = true;	

		    } else {
			   ERR_print_errors_fp(stderr);
		        return -1;
		    }
	    }

	    do {
		    printf("Try reading...\n");
		    bytes = SSL_read(ssl, buf, 1024); /* get request */
		    printf("Data length:%d\n",bytes);
		    if ( bytes > 0 ) {
			  buf[bytes] = 0;
			  //printf("Client msg: \"%s\"\n", buf);
			  
			  http_parser* aParser = &aCtx->_parser;	

			  /* Start up / continue the parser.
			  * Note we pass recved==0 to signal that EOF has been recieved.
			  */
			  int nparsed = http_parser_execute(aParser, &settings, buf, bytes);

			  if (aParser->upgrade) {
			    /* handle new protocol */
			  } else if (nparsed != bytes) {
			    /* Handle error. Usually just close the connection. */
			  }
 	

		    } else {
			 ERR_print_errors_fp(stderr);
		    }	

		    int aSSLError = SSL_get_error(ssl, bytes);
		
		    print_SSL_error(aSSLError);

		    if (aSSLError==SSL_ERROR_ZERO_RETURN || aSSLError==SSL_ERROR_SYSCALL) { // connection closed
				printf("Connection closed.");
				bytes=-1;
				break;
		    }

		    printf("ssl_pending: %d\n", SSL_pending(ssl));		

		    if (aCtx->_has_build_reply) {
				break;
		    }
		   				
	    }
	    while (1);				

	    printf("Return back from read\n");	

	    return bytes;	

	}

	int http_secured_server::run(void) {   

		int sock;

	    fd_set active_fd_set, read_fd_set;	
	    struct sockaddr_in clientname;	

	    SSL_CTX *ctx;

	    std::string aCertFile = http_properties::getInstance().get("ssl.certificate.file");
         std::string aKeyFile = http_properties::getInstance().get("ssl.key.file");	

	    const char* CertFile = aCertFile.c_str();
	    const char* KeyFile = aKeyFile.c_str();

	    SSL_library_init();

	    ctx = InitServerCTX();        /* initialize SSL */
	    LoadCertificates(ctx, CertFile, KeyFile); /* load certs */

	    sock = make_socket(atoi(_portNumber.c_str()));    /* create server socket */	

	    //set_nonblock(sock);	
	   
         /* Initialize the set of active sockets. */
	    FD_ZERO (&active_fd_set);
	    FD_SET (sock, &active_fd_set);

	    while (1)
	    {   

		   /* Block until input arrives on one or more active sockets. */
		   read_fd_set = active_fd_set;
		   printf("Waiting socket event ...\n");
		   if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
			perror ("select");
			exit (EXIT_FAILURE);
		   }

		   printf("Input arrives ...\n");

		   /* Service all the sockets with input pending. */
		   for (int i = 0; i < FD_SETSIZE; ++i) {
		   	
               if (FD_ISSET (i, &read_fd_set)) {

		       if (i == sock) {
				
				   struct sockaddr_in addr;
		   		   socklen_t len = sizeof(addr);
		   		   SSL *ssl;	

				   int client = accept(sock, (struct sockaddr*)&addr, &len);  /* accept connection as usual */


				   FD_SET (client, &active_fd_set);	
				   //set_nonblock(client);

				   printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
				   ssl = SSL_new(ctx);              /* get new SSL state with context */
				   SSL_set_accept_state(ssl);
				   SSL_set_fd(ssl, client);      /* set connection socket to SSL state */
				   
				   // Servlet(ssl);         /* service connection */

				   recordParserSocket(client, ssl, std::string(inet_ntoa(addr.sin_addr)));	 
	
			  } else {
		           /* Data arriving on an already-connected socket. */
		           if (read_from_client(i) < 0) {


					printf("Closing client socket\n");
					http_context* aCtx = &_contexts[i];	
	    				SSL_free(aCtx->_ssl);         /* release SSL state */
		               close (i); /* close connection */
		               FD_CLR (i, &active_fd_set);

		           }
		       }
		    }

		   } 	

	    }
	    close(sock);          /* close server socket */
	    SSL_CTX_free(ctx);         /* release context */
	}

}
