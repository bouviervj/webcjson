// WEBCJSON-RPC Calls library _ JBV 2013/2014 _
// 

Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};

var webcjson_rpc = function(iService,iCallback){  // RPC constructor
    
    this.service=iService; 
    this.initCallback=iCallback;
    this.init();
    this.debug = false;
}

webcjson_rpc.prototype.setDebug = function(){
    this.debug = true;
}

webcjson_rpc.prototype.init=function(){ 
        
	var data = this.call(this.service,"",{}, function(iObject, iData) {
		
		for (var key in iData) {
	
			var num = iData[key];
			var params = "";
			var assign = "";
			var functionString = "";
			if (num>0) {

				for (var i=0; i<num-1; i++){
					params += "arg"+i+" ,";
					assign += "params['arg"+i+"']=arg"+i+";";
				}
				params += "arg"+(num-1);
				assign += "params['arg"+(num-1)+"']=arg"+(num-1)+";";

				functionString = "iObject."+key+" = function("+params+",iCallback) { var params={}; "+assign+" return this.callFunction('"+key+"',params,iCallback); }";

			} else {
				
				functionString = "iObject."+key+" = function(iCallback) { var params={}; "+assign+" return this.callFunction('"+key+"',params,iCallback); }";

			}			
			
			console.log(functionString);
			eval(functionString); // add the function to the object
					
		}
	
		if (!(typeof iObject.initCallback==="undefined")) {
			iObject.initCallback();
		}

	});
		
}

webcjson_rpc.prototype.call=function(iService,iFunc,iParams,iCallback){
		
	console.log("webjson-rpc call ...");

	// Build header
	var sentheader = {
		_function: iFunc,
		_nargs: Object.size(iParams)
	};
	
	for (param in iParams) {
		sentheader[param]=iParams[param];
	}
	
	var self = this;
	// send ajax query
	$.ajaxSetup({cache:false});
	$.ajax({
		url: iService,
		contentType: 'application/json',
		dataType: "jsonp",
		data: JSON.stringify(sentheader),
		processData: false,
		type: 'POST',
		success: function( data ) {
			if (!(typeof iCallback==="undefined")) {
				iCallback( self , data.Response);
			}
		},
		headers : {},
		error: function(jqXHR, textStatus, errorThrown){
			console.log("webjson-rpc: error calling ajax :"+textStatus+":"+errorThrown);
			console.log(jqXHR.responseText);

			if (self.debug) {
				var win=window.open('');
				win.document.write(jqXHR.responseText);
				win.document.close();
			}

		}
	});
	
	return 0;
}

webcjson_rpc.prototype.callFunction=function(iFunction,iParameters,iCallback){
	return this.call(this.service,iFunction,iParameters,iCallback);
}
