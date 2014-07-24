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
#include "http_server.h"

#include "service_loader.h"

namespace webcjson {

	http_server::http_server(const std::string& iPortNumber){
		
		_portNumber = iPortNumber;

		initEngine();
	     initHttpParserSettings();
	
	}

	int http_server::make_socket (uint16_t port) {
		  int sock;
		  struct sockaddr_in name;

		  /* Create the socket. */
		  sock = socket (PF_INET, SOCK_STREAM, 0);
		  if (sock < 0)
		    {
			 perror ("socket");
			 exit (EXIT_FAILURE);
		    }

		  /* Give the socket a name. */
		  name.sin_family = AF_INET;
		  name.sin_port = htons (port);
		  name.sin_addr.s_addr = htonl (INADDR_ANY);
		  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
		    {
			 perror ("bind");
			 exit (EXIT_FAILURE);
		    }
		  if ( listen(sock, 1) != 0 )
	        {
		   perror("Can't configure listening port");
		   abort();
	    	  }
		 return sock;
	}

	int http_server::read_from_client (int filedes)
	{
	  char buffer[MAXMSG];
	  int nbytes;

	  nbytes = read (filedes, buffer, MAXMSG);
	  if (nbytes < 0)
	    {
		 /* Read error. */
		 perror ("read");
		 exit (EXIT_FAILURE); // TODO handle failure (socket closed for example)
	    }
	  else 
	    {

		 http_parser* aParser = &_contexts[filedes]._parser;	

		 /* Start up / continue the parser.
		 * Note we pass recved==0 to signal that EOF has been recieved.
		 */
		 int nparsed = http_parser_execute(aParser, &settings, buffer, nbytes);

		 if (aParser->upgrade) {
		   /* handle new protocol */
		 } else if (nparsed != nbytes) {
		   /* Handle error. Usually just close the connection. */
		 }

		 /* Data read. */
		 //fprintf (stderr, "Server: got message: `%s'\n", buffer);
		 if (nbytes==0){
			return -1;
		 }
		 return 0;

	    }
	}

	// instance dependant methods
	int http_server::on_message_begin_server(http_parser* iParser){
		//fprintf (stdout,"MESSAGE BEGIN\n");
		
		http_context* _context = (http_context*) iParser->data;

		// Query
		_context->_url =  "";
		_context->_lastkey = "";
		_context->_body = "";
		_context->_key_value.clear();

		// Reply
		_context->_extension_reply = "";
		_context->_reply_code = 400;
		_context->_key_value_reply.clear();
		_context->_body_reply = "";

		_context->_has_build_reply = false;

		return 0;
	} 

	int http_server::on_headers_complete_server(http_parser* iParser){
		//fprintf (stdout,"HEADER ENDS\n");
		return 0;
	}

	int http_server::build_reply(http_context* aCtx, std::string& ioStr){

		// Pre process
		// Parses the url
		aCtx->_parsed_url_result = http_parser_parse_url(aCtx->_url.c_str(), aCtx->_url.size(),
						          aCtx->_connect,
						          &aCtx->_parsed_url);

		aCtx->extractCookies();

		// Proceed context treatment in the engine
		aCtx->_server->_engine->proceedQuery(aCtx);

		// Post Process
		aCtx->_connect = 0; // unset connect

		ioStr = webcjson::http_format(aCtx);

		aCtx->_has_build_reply = true;

		aCtx->deleteCookies();

	}

	int http_server::on_message_complete_server(http_parser* iParser){
		//fprintf (stdout,"MESSAGE COMPLETE\n");
		http_context* _context = (http_context*) iParser->data;

		std::string aReply;
		build_reply( _context , aReply);

		//fprintf (stdout,"Reply:%s\n",aReply.c_str());

		ssize_t result = write(_context->_sock, aReply.c_str(), aReply.size()); 
	
		return 0;
	}

	int http_server::on_url_server(http_parser* iParser, const char *at, size_t length){
		//fprintf (stdout,"URL\n");
		http_context* _context = (http_context*) iParser->data;
		_context->_url =  std::string(at,length);
		return 0;
	}

	int http_server::on_header_field_server(http_parser* iParser, const char *at, size_t length){
		//fprintf (stdout,"HEADER FIELD\n");
		http_context* _context = (http_context*) iParser->data;
		_context->_lastkey =  std::string(at,length);
		return 0;
	}

	int http_server::on_header_value_server(http_parser* iParser, const char *at, size_t length){
		//fprintf (stdout,"HEADER VALUE\n");
		http_context* _context = (http_context*) iParser->data;
		_context->_key_value[_context->_lastkey] =  std::string(at,length);
		//fprintf (stdout, "Adding (key,value)=(%s,%s)\n",_context->_lastkey.c_str(), _context->_key_value[_context->_lastkey].c_str());
		return 0;
	}

	int http_server::on_body_server(http_parser* iParser, const char *at, size_t length){
		//fprintf (stdout,"BODY\n");
		http_context* _context = (http_context*) iParser->data;
		_context->_body = std::string(at,length);
		//fprintf (stdout,"%s\n",_context->_body.c_str());
		return 0;
	}

	int http_server::initEngine(){

		// Here we want to find defined port to start the server
		//std::string aPortStr = http_properties::getInstance().get("http_port"); // in future we can have https_port
		if (_portNumber.compare("")!=0) {
			_port = atoi(_portNumber.c_str());
			fprintf (stdout,"Will start server with port (http_port) : %d\n",_port);
		} else {
			fprintf (stdout,"Server port undefined in properties file (http_port) set to default: %d\n",_port);
		}

		_engine = new engine();
		service_loader::getInstance().addEngine(_engine);

		//webcjson::engine::getInstance().getServiceRecorder().loadServices();

	}

	int http_server::initHttpParserSettings(){

		settings.on_message_begin = on_message_begin_server;
		settings.on_headers_complete = on_headers_complete_server;
		settings.on_message_complete = on_message_complete_server;

		settings.on_url = on_url_server;
		settings.on_header_field = on_header_field_server;
		settings.on_header_value = on_header_value_server;
		settings.on_body = on_body_server;

		for (int i=0; i<FD_SETSIZE; i++) {
			_contexts[i]._connect = 0;
		}

	}

	int http_server::recordParserSocket(int iSock, const std::string& iIP){

		http_parser_init(&_contexts[iSock]._parser, HTTP_REQUEST);
		_contexts[iSock]._sock = iSock;
		_contexts[iSock]._server = this;
		_contexts[iSock]._parser.data = (void*) &_contexts[iSock];
		_contexts[iSock]._connect = 1;
		_contexts[iSock]._secured = false;
		_contexts[iSock]._ip = iIP;

	}

	pthread_t& http_server::start(){
	
		pthread_create(&_thread, NULL, &http_server::staticThread, this);
		return _thread;

	}

	void* http_server::staticThread(void *arg)
	{
	    return (void*) (long) reinterpret_cast<http_server*>(arg)->run();
	}

	int http_server::run (void)
	{
	  //extern int make_socket (uint16_t port);
	  int sock;
	  fd_set active_fd_set, read_fd_set;
	  int i;
	  struct sockaddr_in clientname;
	  size_t size;
	 
	  /* Create the socket and set it up to accept connections. */
	  sock = make_socket (_port); // port defined in initEngine
	 
	  /* Initialize the set of active sockets. */
	  FD_ZERO (&active_fd_set);
	  FD_SET (sock, &active_fd_set);

	  while (1)
	    {
		 /* Block until input arrives on one or more active sockets. */
		 read_fd_set = active_fd_set;
		 if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
		   {
		     perror ("select");
		     exit (EXIT_FAILURE);
		   }

		 /* Service all the sockets with input pending. */
		 for (i = 0; i < FD_SETSIZE; ++i)
		   if (FD_ISSET (i, &read_fd_set))
		     {
		       if (i == sock)
		         {
		           /* Connection request on original socket. */
		           int newSock;
		           size = sizeof (clientname);
		           newSock = accept (sock,
		                         (struct sockaddr *) &clientname,
		                         (socklen_t*) &size);
		           if (newSock < 0)
		             {
		               perror ("accept");
		               exit (EXIT_FAILURE);
		             }
		           /*fprintf (stderr,
		                    "Server: connect from host %s, port %d.\n",
		                    inet_ntoa (clientname.sin_addr),
		                    ntohs (clientname.sin_port));*/
		           FD_SET (newSock, &active_fd_set);

				 recordParserSocket(newSock,std::string(inet_ntoa (clientname.sin_addr)));

		         }
		       else
		         {
		           /* Data arriving on an already-connected socket. */
		           if (read_from_client (i) < 0)
		             {
		               close (i);
		               FD_CLR (i, &active_fd_set);
		             }
		         }
		     }
	    }
	}


}
