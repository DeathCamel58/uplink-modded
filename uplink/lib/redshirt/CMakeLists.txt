project(redshirt CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

include_directories(${UNRAR_INCLUDE_DIR})
include_directories(${BUNGLE_INCLUDE_DIR})
include_directories(${TOSSER_INCLUDE_DIR})
include_directories(${MMGR_INCLUDE_DIR})

FILE(GLOB redshirtSRC *.cpp)
add_library(redshirt ${redshirtSRC})

set(REDSHIRT_INCLUDE_DIR ${PROJECT_SOURCE_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)