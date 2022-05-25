project(soundgarden CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

include_directories(${TOSSER_INCLUDE_DIR})
include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDL_MIXER_INCLUDE_DIR})
include_directories(${MMGR_INCLUDE_DIR})
include_directories(${REDSHIRT_INCLUDE_DIR})
include_directories(${ECLIPSE_INCLUDE_DIR})
include_directories(${GUCCI_INCLUDE_DIR})

message("Soundgarden Configuration:")

# Use SDL
option(SOUNDGARDEN_USE_SDL "Use SDL" ON)
if (SOUNDGARDEN_USE_SDL)
    add_definitions(-DUSE_SDL)
    include_directories(${SDL_MIXER_INCLUDE_DIR})
    include_directories(${SDL_INCLUDE_DIR})
endif ()
message("-- SOUNDGARDEN_USE_SDL: '${SOUNDGARDEN_USE_SDL}'")

# Debug option
option(SOUNDGARDEN_DEBUG "Enable Debugging" OFF)
if (SOUNDGARDEN_DEBUG)
    add_definitions(-D_DEBUG)
endif ()
message("-- SOUNDGARDEN_DEBUG: '${SOUNDGARDEN_DEBUG}'")

FILE(GLOB soundgardenSRC *.cpp)
add_library(soundgarden ${soundgardenSRC})

set(SOUNDGARDEN_INCLUDE_DIR ${PROJECT_SOURCE_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
