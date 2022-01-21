cmake_minimum_required (VERSION 3.0.0)
project (uplinkLibs)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

# Although `binreloc` is required, it isn't a library. It's used to generate a `cpp` and `h` file that goes into the
# source tree.
#FILE(GLOB binrelocSRC binreloc/*.c)
#add_library(binreloc ${binrelocSRC})
#include_directories(binreloc)

# Use vcpkg libraries
if(WIN32)
    find_package(freetype CONFIG REQUIRED)
    find_package(harfbuzz CONFIG REQUIRED)
    find_package(libxml2 CONFIG REQUIRED)
    find_package(harfbuzz CONFIG REQUIRED)
    find_package(sdl2 CONFIG REQUIRED)
    find_package(sdl2-mixer CONFIG REQUIRED)
else()
    add_subdirectory(freetype)
    set(FREETYPE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/freetype/include)

    add_subdirectory(SDL)
    set(SDL_INCLUDE_DIR
            ${PROJECT_SOURCE_DIR}/SDL/src
            ${PROJECT_SOURCE_DIR}/SDL/include
            CACHE INTERNAL "SDL_mixer: Include Directories" FORCE)

    add_subdirectory(SDL_mixer)
    set(SDL_MIXER_INCLUDE_DIR
            ${PROJECT_SOURCE_DIR}/SDL_mixer
            ${PROJECT_SOURCE_DIR}/SDL_mixer/include
            CACHE INTERNAL "SDL_mixer: Include Directories" FORCE)

    find_package(LibXml2 REQUIRED)
    include_directories(${LIBXML2_INCLUDE_DIR})
endif()

add_subdirectory(zlib)

add_subdirectory(unrar)

# Can't migrate ftgl to use cmake, as it's a submodule, and that change would have to be upstreamed
# This patches ftgl before compilation
# Reference `uplink/src/data/docs/FTGL.patch` for original patches
# Reference FTGL bug #36 <https://sourceforge.net/p/ftgl/bugs/36/> for details
file(READ ftgl/src/FTSize.cpp FILE_CONTENTS)
string(REPLACE "xResolution, yResolution" "xRes, yRes" FILE_CONTENTS_NEW "${FILE_CONTENTS}")
file(WRITE ftgl/src/FTSize.cpp "${FILE_CONTENTS_NEW}")
unset(FILE_CONTENTS)
unset(FILE_CONTENTS_NEW)
add_subdirectory(ftgl)
set(FTGL_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/ftgl/src
        ${PROJECT_SOURCE_DIR}/ftgl/src/FTFont
        ${PROJECT_SOURCE_DIR}/ftgl/src/FTGL
        ${PROJECT_SOURCE_DIR}/ftgl/src/FTGlyph
        ${PROJECT_SOURCE_DIR}/ftgl/src/FTLayout
        CACHE INTERNAL "ftgl: Include Directories" FORCE)

add_subdirectory(irclib)

#add_subdirectory(xz)

#find_package(LibLZMA REQUIRED)
#include_directories(${LIBLZMA_INCLUDE_DIRS})
#add_subdirectory(libtiff)

add_subdirectory(tcp4u.331)

add_subdirectory(mmgr)

add_subdirectory(tosser)

add_subdirectory(bungle)

add_subdirectory(gucci)

add_subdirectory(eclipse)

add_subdirectory(redshirt)

add_subdirectory(soundgarden)

add_subdirectory(vanbakel)
