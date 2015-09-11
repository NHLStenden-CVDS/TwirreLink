cmake_minimum_required ( VERSION 2.6 ) 
project ( Twirre-v2-Library )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -msse4 -pedantic -Wall -Wextra" )


set (CMAKE_C_COMPILER clang )
  set (CMAKE_CXX_COMPILER clang++ )
  
  SET (CMAKE_AR      "llvm-ar")
  SET (CMAKE_LINKER  "llvm-ld")
  SET (CMAKE_NM      "llvm-nm")
  SET (CMAKE_OBJDUMP "llvm-objdump")
  SET (CMAKE_RANLIB  "llvm-ranlib")

include_directories ( ${PROJECT_SOURCE_DIR} )
include_directories ( "${PROJECT_SOURCE_DIR}/Core" )
include_directories ( "${PROJECT_SOURCE_DIR}/Serial" )
include_directories ( "${PROJECT_SOURCE_DIR}/Support" )
include_directories ( "${PROJECT_SOURCE_DIR}/TwirreSerial" )


file( GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/*.cpp" )

add_library( twirrelink SHARED ${SRC_FILES} )	
