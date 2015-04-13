cmake_minimum_required ( VERSION 2.6 ) 
project ( Twirre-v2-Library )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -mavx -pedantic -Wall -Wextra" )

include_directories ( ${PROJECT_SOURCE_DIR} )
include_directories ( "${PROJECT_SOURCE_DIR}/Core" )
include_directories ( "${PROJECT_SOURCE_DIR}/Serial" )
include_directories ( "${PROJECT_SOURCE_DIR}/Support" )
include_directories ( "${PROJECT_SOURCE_DIR}/TwirreSerial" )

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/*.cpp" )

add_executable(Twirre-v2-Library ${SRC_FILES} )	
