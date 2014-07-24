CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

file(GLOB common_sources ./*.cpp)

include_directories( ./ )
include_directories( ./utils/ )

SET(CC_COVERAGE_COMPILE_FLAGS "-fPIC -g")
SET(CC_COVERAGE_LINK_FLAGS    "")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CC_COVERAGE_COMPILE_FLAGS} ${CC_COVERAGE_LINK_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CC_COVERAGE_LINK_FLAGS}" ) 

#add_library (dictionary SHARED ${common_sources} )
#target_link_libraries ( dictionary ${CMAKE_DL_LIBS} )
