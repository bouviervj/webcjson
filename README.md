webcjson
========


C/C++ JSON Web Service
======================
This project aims at deploying easily JSON web services on C/C++ development environment.
It uses https://github.com/bouviervj/cjson cjson project to make the interface between C /C++ and JSON.
The web server support basic HTTP 1.1, cookies, compression and can handle SSL connections.

Basic idea is that you can deploy easily JSON web services, with simple js RPC library. As  cjson project, it uses GCCXML project to get description on JSON serialization, stub generation, and session context storage. When using this project there's *no need to write C/C++ JSON serialization wrappers*: it is automatically done by cjson library.

Features
========
 * Embedded C/C++ webserver
 * JSON Web Services
 * JSON Session Context
 * Javascript RPC
 * HTTP 1.1
 * Cookies
 * Frames compression
 * Etags
 * SSL connections
 
 

TODO List
=========
 * Database support: sql/nosql
 * Users management


Example
=======
An example to deploy Fibonacci methods, by defining this header:

```C
#ifndef __MODULE_FIBO_H__
#define __MODULE_FIBO_H__

#include "../lib/engine/rpc_module.h"

class module_fibo : public webcjson::rpc_module {


	public:	

		int fibonacci(int n) {
		   if (n < 2){
			return n;
		   } else {
			return fibonacci(n-2) + fibonacci(n-1);
		   }
		}


		int fibonacci_it(int n){
		  int u = 0;
		  int v = 1;
		  int i, t;
		  for(i = 2; i <= n; i++) {
		    t = u + v;
		    u = v;
		    v = t;
		  }
		  return v;
		}


};

#endif 
```

The module_fibo class inherits from webcjson::rpc_module. By building XML description of this class by GCCXML, the stubber is able to create a loadable module to deploy a JSON webservice, here is the created header stub :

```C
#ifndef _SERVICE_MODULE_FIBO_H__
#define _SERVICE_MODULE_FIBO_H__

#include <rpc_module.h>
#include <recorded_service.h>
#include <module_fibo.h>

namespace webcjson {

	class service_module_fibo : public module_service<module_fibo > { 

		public:

			service_module_fibo(const std::string& iURL) : module_service<module_fibo>(iURL) {
			}

			virtual int execute(const std::string& iFunction, const std::vector<std::string>& iArgs, std::string& ioResult);

			virtual void returnDefinition(std::string& ioDefinition);

			ADD_RECORDED_SERVICE(service_module_fibo);

	};

}

#endif

```

After compiling this generated stub, when launching webcjson project, the service is deployed automatically:

```C
jbv@jbv-Aspire-4820TG:~/projects/webcjson$ ./webcjson
Will start server with port (http_port) : 5555
Will start server with port (http_port) : 5556
Trying to load service libstubbed.so...
 Declaration: /module_fibo
 Declaration: /module_context
Exiting from load service
Waiting socket event ...
```

Then in your HTML code, also deployed by the server, you have to include webcjson-rpc.js file in your headers and use it like that :

```C
<html>
<head>
<title>Welcome under Webcjson</title>
<script src="./js/jquery-1.10.2.min.js"></script>
<script src="./js/webcjson-rpc.js"></script>
 <script>

	$(function() {
		
                /* RPC Module initialization */
		var aRPC = new webcjson_rpc("/module_fibo", function(){

			// Calling fibonacci method 1 
			aRPC.fibonacci(11, function(iObject,iResult){
				$("#fibonacci").html("fibonacci(11)="+iResult);
			});

                        // Calling fibonacci iterative method 2 
			aRPC.fibonacci_it(17, function(iObject,iResult){
				$("#fibonacci_it").html("fibonacci_it(17)="+iResult);
			});


		});

	});

</script>
</head>
<body>
Welcome under Webcjson
<div id="fibonacci"></div>
<div id="fibonacci_it"></div>
</body>
```

The final result in the HTML page is : 

```
Welcome under Webcjson
fibonacci(11)=89
fibonacci_it(17)=1597
```


