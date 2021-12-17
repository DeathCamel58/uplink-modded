project(tcp4u CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

if (UNIX)
    add_definitions(-DUNIX)
endif ()
add_definitions(-D${CMAKE_HOST_SYSTEM_NAME})
include_directories(Include)
FILE(GLOB tcp4uSRC src/*.c)
add_library(tcp4u ${tcp4uSRC})

set(TCP4U_INCLUDE_DIR ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/Include CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)