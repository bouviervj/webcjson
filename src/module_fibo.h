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
