
echo "Building files with GCCXML ..."
gccxml src/module_all.h --gccxml-gcc-options gccxml.options -fxml=./etc/grammar_without_dtd.xml
gccxml src/databases/webcjson_databases.h --gccxml-gcc-options gccxml.options -fxml=./etc/databases_grammar_without_dtd.xml
echo "Adding dtd to xml dictionary ..."
xsltproc -o ./etc/grammar.xml ./etc/adddtd.xsl ./etc/grammar_without_dtd.xml
xsltproc -o ./etc/databases_grammar.xml ./etc/adddtd.xsl ./etc/databases_grammar_without_dtd.xml
rm ./etc/grammar_without_dtd.xml
rm ./etc/databases_grammar_without_dtd.xml
