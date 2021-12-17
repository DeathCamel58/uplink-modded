project(mmgr CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

FILE(GLOB mmgrSRC *.cpp)
add_library(mmgr ${mmgrSRC})

set(MMGR_INCLUDE_DIR ${PROJECT_SOURCE_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)