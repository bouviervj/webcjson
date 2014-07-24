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

#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include "./lib/utils/Properties.h"
#include "./lib/cjson_dictionary.h"
#include "./lib/utils/utils.h"
#include "./lib/utils/files.h"
#include "./lib/utils/advanced_string.h"

#include <vector>

using namespace cjson;

conf::Properties* _properties;
cjson::dictionary* _dictionary;

class module_item {
	public:
		field::class_field* _class;
		bool _session;
};

int init(){
	
	// Init properties file
	std::string iPropertiesFileName("./etc/conf.properties");
	_properties = new conf::Properties(iPropertiesFileName);

	// Init dictionary
	std::cout << "Loading grammar :" << (*_properties)["modules.grammar"] << "\n";
	_dictionary = new cjson::dictionary((*_properties)["modules.grammar"]);

	return 0;
}

int listModules(std::vector<module_item>& ioList){

	std::map<std::string, field::field* > aList = _dictionary->getResources();

	for (std::map<std::string, field::field*>::iterator it=aList.begin(); it!=aList.end(); ++it) { // loop on classes elements
    		//std::cout << it->first << '\n';

		field::class_field* aClass = dynamic_cast<field::class_field*>(it->second);

		if (aClass) {

			// Here we want to detect if this class has to be stubbed
			const cjson::ABI_Inh* aABI =  &aClass->getABI();

			const std::vector<ABI_Inheritance_Struct>* aList = &aABI->_inherited;
			for (int i=0; i<aList->size(); i++) {				
				if ((*aList)[i]._name=="webcjson::rpc_module") {

					module_item aItem;
					aItem._class = aClass;
					aItem._session = false;			
			
					std::cout<<"inh:"<<(*aList)[i]._name;
					ioList.push_back(aItem);
					std::cout<<" => will be stubbed";
					std::cout << std::endl;

				} else if (adv::startWidth((*aList)[i]._name, "webcjson::rpc_session_module")) {

					module_item aItem;
					aItem._class = aClass;
					aItem._session = true;		

					std::cout<<"inh:"<<(*aList)[i]._name;
					ioList.push_back(aItem);
					std::cout<<" => will be stubbed";
					std::cout << std::endl;
				}
				
			}


		}

	}

	return 0;

}

int buildModuleHeader(const std::string& iFileName, field::class_field* iList, bool iSession){
	
	std::cout<<"Creating header: "<<iFileName << ".h" <<std::endl;

	std::string aH = iFileName;
	uppercase(aH);

	// TODO add information

	std::ostringstream oStream;
	oStream << "#ifndef _" << aH << "_H__" <<std::endl;
	oStream << "#define _" << aH << "_H__" <<std::endl;
	oStream << std::endl;

	// INCLUDES
	oStream <<  "#include <rpc_module.h>" << std::endl;
	oStream <<  "#include <recorded_service.h>" << std::endl;
	oStream <<  "#include <"<< iList->getName() <<".h>" << std::endl;
	oStream << std::endl;

	// ENTERING NAMESPACE
	oStream << "namespace webcjson {" <<std::endl;
	oStream << std::endl;

	// CREATING CLASS DEFINITION
	if (iSession) {
		oStream << "\tclass " << iFileName << " : public module_session_service<"<< iList->getName() <<" > { "<<std::endl;
	} else {
		oStream << "\tclass " << iFileName << " : public module_service<"<< iList->getName() <<" > { "<<std::endl;
	}	
	oStream << std::endl;
	oStream << "\t\tpublic:" << std::endl; 
	oStream << std::endl;
	if (iSession) {
		oStream << "\t\t\t" << iFileName << "(const std::string& iURL) : module_session_service<"<<  iList->getName() <<">(iURL) {"<<std::endl;
	} else {
		oStream << "\t\t\t" << iFileName << "(const std::string& iURL) : module_service<"<<  iList->getName() <<">(iURL) {"<<std::endl;
	}
	oStream << "\t\t\t}"<<std::endl;
	oStream << std::endl;
	oStream << "\t\t\tvirtual int execute(const std::string& iFunction, const std::vector<std::string>& iArgs, std::string& ioResult);"<<std::endl;
	oStream << std::endl;
	oStream << "\t\t\tvirtual void returnDefinition(std::string& ioDefinition);"<<std::endl;
	oStream << std::endl;
	oStream << "\t\t\tADD_RECORDED_SERVICE("<< iFileName <<");"<<std::endl;
	oStream << std::endl;
	
	oStream << "\t};" << std::endl;
	oStream << std::endl;

	// CLOSING NAMESPACE
	oStream << "}" <<std::endl;
	oStream << std::endl;

	oStream << "#endif" <<std::endl;

	std::cout << oStream.str();
	files::writeFile("./stubs/"+iFileName+".h", oStream.str());

}



int buildModuleCode(const std::string& iFileName, field::class_field* iClass, bool iSession){

	std::cout<<"Creating cpp file: "<<iFileName << ".cpp" <<std::endl;

	std::ostringstream oStream;

	// INCLUDES
	oStream <<  "#include <"<< iFileName <<".h>" << std::endl;
	oStream <<  "#include <http_grammar.h>" << std::endl;
	oStream <<  "#include <jsonxx.h>" << std::endl;
	oStream <<  "#include <string>" << std::endl;
	oStream << std::endl;

	// ENTERING NAMESPACE
	oStream << "namespace webcjson {" <<std::endl;
	oStream << std::endl;

	// CREATING METHODS
	std::string aUrl = "/" + iClass->getName(); 
	oStream << "\t" << "CREATE_RECORDED_SERVICE(" << iFileName << ",\"" << aUrl << "\")" << std::endl;
	oStream << std::endl;

	oStream << "\t" << "int "<< iFileName <<"::execute(const std::string& iFunction, const std::vector<std::string>& iArgs, std::string& ioResult){" << std::endl;
	oStream << std::endl;

	oStream <<"\t\tstd::cout << \" Execute "<< iFileName <<" method.\" << std::endl ; " << std::endl;
	oStream << std::endl;

	const cjson::ABI_Inh* aABI =  &iClass->getABI();
	std::cout << "nb methods:" << aABI->_methods.size() <<std::endl;
	bool first = true;
	bool last = false;
	for (int i=0; i<aABI->_methods.size(); i++) {

		std::cout << "methods:" << i <<std::endl;

		const ABI_Method_Element* aMethod = &aABI->_methods[i];

		if (!aMethod->_operator) {

			if (!first) {
				oStream <<" else ";
			} else {
				oStream <<"\t\t";
			}

			oStream <<"if (iFunction==\""<< aMethod->_name <<"\") {" << std::endl;
			oStream << std::endl;

			// VARIABLES DECLARATION
			for (int j=0; j< aMethod->_arguments.size(); j++) {
				oStream << "\t\t\t" << aMethod->_arguments[j] << "* aArg" << j << "=0;" << std::endl;
			}
			oStream << std::endl;

			oStream << "\t\t\ttry {" << std::endl; // catch exception TODO manage leak strategy if (aArg0) delete aArg0
			oStream << std::endl;

			// ALLOCATE
			for (int j=0; j< aMethod->_arguments.size(); j++) {
				oStream << "\t\t\t\t" << "aArg" << j << "=http_grammar::getInstance().getDictionary()->fromJson<" << aMethod->_arguments[j] << ">(iArgs["<< j <<"]);" << std::endl;
			}
			oStream << std::endl;
		
			// CALL
			oStream << "\t\t\t\t" <<  aMethod->_return << " aReturn = " << "_module."<< aMethod->_name << "(" ;			
			for (int j=0; j< aMethod->_arguments.size(); j++) {
				oStream << "*aArg" << j ;
				if (j!=aMethod->_arguments.size()-1) oStream << ",";
			}	
			oStream << ");" << std::endl;
			oStream << std::endl;

			// FREE arguments
			for (int j=0; j< aMethod->_arguments.size(); j++) {
				oStream << "\t\t\t\t" << "delete aArg" << j << ";" << std::endl;
			}
			oStream << std::endl;

			// BUILD RESULT
			oStream << "\t\t\t\t" << "ioResult = http_grammar::getInstance().getDictionary()->toJson<"<< aMethod->_return <<">(&aReturn);" <<std::endl;
			oStream << std::endl;

			// CATCH Exceptions
			oStream << "\t\t\t} catch (jsonxx::jsonxx_exception const & ex) {" << std::endl; // catch exception TODO manage leak strategy if (aArg0) delete aArg0
			oStream << std::endl;
		
			oStream << "\t\t\t\t//Building error stream" << std::endl;	
			oStream << "\t\t\t\tstd::ostringstream oStream;" << std::endl;	
    			oStream << "\t\t\t\toStream << ex.what() << std::endl; " << std::endl;	  
    			oStream << std::endl;

			oStream << "\t\t\t\t//Have to return bad request or body containing what()" << std::endl;	

			// FREE arguments
			for (int j=0; j< aMethod->_arguments.size(); j++) {
				oStream << "\t\t\t\tif (aArg" << j << "){" << std::endl;
			     oStream << "\t\t\t\t\tdelete aArg" << j << ";"<<std::endl;
				oStream << "\t\t\t\t} else {" << std::endl;
				oStream << "\t\t\t\t\toStream << \""<<  aMethod->_name <<": arg"<< j <<" ("<< aMethod->_arguments[j] <<") not defined correctly received \" << iArgs["<< j <<"] << \".\\n\" << std::endl;" << std::endl;
				oStream << "\t\t\t\t}" << std::endl;
			}
			oStream << std::endl;

			oStream << "\t\t\t\tioResult = oStream.str(); " << std::endl;	
			oStream << std::endl;

			oStream << "\t\t\t\treturn 1;" << std::endl;		

			oStream << "\t\t\t}" << std::endl;
			oStream << std::endl;

			oStream << "\t\t}" ;
			

			first = false;

		}

	}
	oStream << std::endl << std::endl;

	oStream << "\t\treturn 0;" << std::endl;
	oStream << std::endl;

	oStream <<"\t}" <<std::endl;
	oStream << std::endl;

	oStream << "\tvoid "<< iFileName <<"::returnDefinition(std::string& ioDefinition){"<<std::endl;
	oStream << "\t\tioDefinition=\"{";
	for (int i=0; i<aABI->_methods.size(); i++) {
		if (!aABI->_methods[i]._operator) {
			oStream << "\\\"" << aABI->_methods[i]._name << "\\\":" << aABI->_methods[i]._arguments.size();
			if (i!=aABI->_methods.size()-1) oStream << ",";
		}
	}
	oStream << "}\";" <<std::endl;
	oStream <<"\t}" <<std::endl;
	oStream << std::endl;


	// CLOSING NAMESPACE
	oStream << "}" <<std::endl;
	oStream << std::endl;

	std::cout << oStream.str(); // Log file

	files::writeFile("./stubs/"+iFileName+".cpp", oStream.str());

}

int buildAllModules(const std::vector<module_item>& iList){
	
	 mkdir("./stubs",0777);

	for (int i=0; i<iList.size(); i++) {
		
		std::string aName = "service_"+iList[i]._class->getName();
		replace(aName, "::", "_");
		std::cout<<"stubbing:"<<aName<<std::endl;

		buildModuleHeader(aName, iList[i]._class, iList[i]._session);
		buildModuleCode(aName, iList[i]._class, iList[i]._session);	

	}

	return 0;
}

int main( int argc, const char* argv[] )
{

	init();

	std::vector<module_item> aList;
	listModules(aList);

	buildAllModules(aList);	

}








