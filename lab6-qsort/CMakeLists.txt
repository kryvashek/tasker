cmake_minimum_required( VERSION 3.0 )
project( lab6_qsort )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11" )
set( EXECUTABLE_OUTPUT_PATH  ${CMAKE_CURRENT_BINARY_DIR}/bin )
include_directories( inc )
set( SOURCE_FILES
        src/main.cpp
        )
add_executable( lab6_qsort ${SOURCE_FILES} )
