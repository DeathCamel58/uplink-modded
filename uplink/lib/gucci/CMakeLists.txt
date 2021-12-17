project(gucci CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

include_directories(${TOSSER_INCLUDE_DIR})
include_directories(${MMGR_INCLUDE_DIR})
include_directories(${FTGL_INCLUDE_DIR})

message("GUCCI Configuration:")

SET(GUCCI_FRAME_RATE "35" CACHE STRING "Frame rate for GUCCI")
message("-- GUCCI_FRAME_RATE: '${GUCCI_FRAME_RATE}'")

# Use SDL
option(GUCCI_USE_SDL "Use SDL" ON)
if (GUCCI_USE_SDL)
    add_definitions(-DUSE_SDL)
endif ()
message("-- GUCCI_USE_SDL: '${GUCCI_USE_SDL}'")

# Use FTGL
option(GUCCI_USE_FTGL "Use FTGL" ON)
if (GUCCI_USE_FTGL)
    add_definitions(-DUSE_FTGL)
    include_directories(${FTGL_INCLUDE_DIR})
    include_directories(${FREETYPE_INCLUDE_DIR})
else(GUCCI_USE_FTGL)
    add_definitions(USE_GLTT)
endif ()
message("-- GUCCI_USE_FTGL: '${GUCCI_USE_FTGL}'")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/gucci_sdl.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/gucci_sdl.cpp")

add_library(gucci gucci.cpp image.cpp  gucci_sdl.cpp)

set(GUCCI_INCLUDE_DIR ${PROJECT_SOURCE_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)