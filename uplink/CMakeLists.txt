cmake_minimum_required (VERSION 3.0.0)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

add_subdirectory(lib)
add_subdirectory(src)
