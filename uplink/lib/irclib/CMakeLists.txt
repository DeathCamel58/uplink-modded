project(irclib CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

include_directories(linux)
FILE(GLOB irclibSRC *.cpp)
FILE(GLOB irclibLinuxSRC linux/*.cpp)
FILE(GLOB irclibHeader *.h)
FILE(GLOB irclibLinuxHeader *.h)
add_library(irclib ${irclibSRC} ${irclibLinuxSRC} ${irclibHeader} ${irclibLinuxHeader})
set_target_properties(irclib PROPERTIES LINKER_LANGUAGE CXX)

set(IRCLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/linux CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)