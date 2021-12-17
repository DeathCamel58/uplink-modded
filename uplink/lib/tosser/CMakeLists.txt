project(tosser CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

include_directories(${MMGR_INCLUDE_DIR})

FILE(GLOB tosserSRC *.cpp)
add_library(tosser ${tosserSRC})

set(TOSSER_INCLUDE_DIR ${PROJECT_SOURCE_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)