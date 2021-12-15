cmake_minimum_required (VERSION 3.10)
project (uplink)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(uplink)