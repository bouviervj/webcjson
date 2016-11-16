CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(webcjson C CXX)

INCLUDE(find_cmake/FindXercesC.cmake)
INCLUDE(find_cmake/FindMySQL.cmake)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
endif( ZLIB_FOUND )

find_package(OpenSSL REQUIRED) 

if ( OPENSSL_FOUND )
	include_directories( ${OPENSSL_INCLUDE_DIR} )
endif( OPENSSL_FOUND ) 

if (MYSQL_FOUND)
	include_directories( ${MYSQL_INCLUDE_DIRECTORIES} )
endif( )

include_directories( ./ )
include_directories( ./lib )
include_directories( ./lib/utils )
include_directories( ./lib/utils/hash-library )
include_directories( ./lib/fields )
include_directories( ./lib/fields/extended )
include_directories( ./lib/fields/extended/stl )
include_directories( ./lib/utils/jsonxx )
include_directories( ./lib/http-parser )
include_directories( ./lib/engine )
include_directories( ./lib/databases )
include_directories( ./lib/databases/drivers )
include_directories( ./lib/databases/drivers/sql )
include_directories( ./lib/databases/drivers/sql/mysql )
include_directories( ./lib/databases/wrappers )
include_directories( ./lib/databases/wrappers/sql )
include_directories( ./stubs )
include_directories( ./src )
include_directories( ./test )

add_subdirectory (lib)

SET(CC_COVERAGE_COMPILE_FLAGS "-fPIC -g -O0")
SET(CC_COVERAGE_LINK_FLAGS    " ")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CC_COVERAGE_COMPILE_FLAGS} ${CC_COVERAGE_LINK_FLAGS} " )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CC_COVERAGE_LINK_FLAGS}" ) 

# Libraries
file(GLOB stubs_sources ./stubs/*.cpp)
file(GLOB lib_sources ./src/*.cpp)

# Common
file(GLOB utils_sources ./lib/utils/*.cpp)
file(GLOB jsonxx_sources ./lib/utils/jsonxx/*.cc)
file(GLOB trex_sources ./lib/utils/trex/*.c)
file(GLOB hash_sources ./lib/utils/hash-library/*.cpp)

# Fields
file(GLOB dicts_sources ./lib/*.cpp)
file(GLOB fields_sources ./lib/fields/*.cpp)
file(GLOB extended_sources ./lib/fields/extended/*.cpp)
file(GLOB stl_sources ./lib/fields/extended/stl/*.cpp)

# Web
file(GLOB httpparser_sources ./lib/http-parser/*.c)
file(GLOB engine_sources ./lib/engine/*.cpp)

# Database
file(GLOB db_sources ./lib/databases/*.cpp)
file(GLOB db_wrappers_sql_sources ./lib/databases/wrappers/sql/*.cpp)
file(GLOB db_driver_sql_sources ./lib/databases/drivers/sql/*.cpp)
file(GLOB db_driver_mysql_sources ./lib/databases/drivers/sql/mysql/*.cpp)


file(GLOB main_sources ./main.cc)
file(GLOB stubber_sources ./stubber.cc)
file(GLOB server_sources ./server.cc)
file(GLOB test_db_wrapper ./test/test_db_wrapper.cc)

add_executable(stubber 
			${stubber_sources} 
			${dicts_sources} 
			${utils_sources} 
			${fields_sources} 
			${extended_sources} 
			${stl_sources} 
			${jsonxx_sources} 
			${trex_sources} )

target_link_libraries(stubber ${CMAKE_DL_LIBS} ${XERCESC_LIBRARY} ${ZLIB_LIBRARIES} -lpthread) 

add_executable(webcjson 
			${server_sources} 
			${httpparser_sources} 
			${fields_sources} 
			${engine_sources}  
			${dicts_sources} 
			${utils_sources} 
			${extended_sources} 
			${stl_sources} 
			${jsonxx_sources} 
			${trex_sources} )
			#${db_wrappers_sql_sources} )
target_link_libraries(webcjson ${CMAKE_DL_LIBS} ${XERCESC_LIBRARY} ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} -lpthread)

add_executable(test_db_wrapper 
			${test_db_wrapper}
			${fields_sources} 
			${dicts_sources} 
			${utils_sources} 
			${extended_sources} 
			${stl_sources} 
			${jsonxx_sources} 
			${trex_sources} 
			${hash_sources}
			${db_sources}
			${db_wrappers_sql_sources}
			${db_driver_sql_sources}
			${db_driver_mysql_sources} )
target_link_libraries(test_db_wrapper ${CMAKE_DL_LIBS} ${XERCESC_LIBRARY} ${ZLIB_LIBRARIES} ${MYSQL_LIBRARY} -lpthread)

add_library(stubbed SHARED ${stubs_sources} ${lib_sources} )
set_target_properties( stubbed
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared"
)



