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

add_subdirectory(freetype)
include_directories(freetype/include)

# This patches ftgl before compilation
# Reference `uplink/src/data/docs/FTGL.patch` for original patches
# Reference FTGL bug #36 <https://sourceforge.net/p/ftgl/bugs/36/> for details
file(READ ftgl/src/FTSize.cpp FILE_CONTENTS)
string(REPLACE "xResolution, yResolution" "xRes, yRes" FILE_CONTENTS_NEW "${FILE_CONTENTS}")
file(WRITE ftgl/src/FTSize.cpp "${FILE_CONTENTS_NEW}")
unset(FILE_CONTENTS)
unset(FILE_CONTENTS_NEW)

add_subdirectory(ftgl)
include_directories(ftgl/src ftgl/src/FTFont ftgl/src/FTGL ftgl/src/FTGlyph ftgl/src/FTLayout)

include_directories(irclib/linux)
FILE(GLOB irclibSRC irclib/*.cpp)
FILE(GLOB irclibLinuxSRC irclib/linux/*.cpp)
FILE(GLOB irclibHeader irclib/*.h)
FILE(GLOB irclibLinuxHeader irclib/*.h)
add_library(irclib ${irclibSRC} ${irclibLinuxSRC} ${irclibHeader} ${irclibLinuxHeader})
set_target_properties(irclib PROPERTIES LINKER_LANGUAGE CXX)
include_directories(irclibSRC)

add_subdirectory(xz)

#find_package(LibLZMA REQUIRED)
#include_directories(${LIBLZMA_INCLUDE_DIRS})
#add_subdirectory(libtiff)

add_subdirectory(SDL)
add_subdirectory(SDL_mixer)

add_definitions(-DUNIX)
add_definitions(-D${CMAKE_HOST_SYSTEM_NAME})
include_directories(tcp4u.331/Include)
FILE(GLOB tcp4uSRC tcp4u.331/src/*.c)
add_library(tcp4u ${tcp4uSRC})

add_subdirectory(zlib)

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

FILE(GLOB unrarSRC unrar/*.cpp)
FILE(GLOB unrarHeaders unrar/*.hpp)
add_definitions(-O2
        -D_FILE_OFFSET_BITS=64
        -D_LARGEFILE_SOURCE
        -DRARDLL)
add_library(unrar ${unrarHeaders} unrar/filestr.cpp unrar/recvol.cpp unrar/rs.cpp unrar/scantree.cpp unrar/rar.cpp unrar/strlist.cpp unrar/strfn.cpp unrar/pathfn.cpp unrar/int64.cpp unrar/savepos.cpp unrar/global.cpp unrar/file.cpp unrar/filefn.cpp unrar/filcreat.cpp unrar/archive.cpp unrar/arcread.cpp unrar/unicode.cpp unrar/system.cpp unrar/isnt.cpp unrar/crypt.cpp unrar/crc.cpp unrar/rawread.cpp unrar/encname.cpp unrar/resource.cpp unrar/match.cpp unrar/timefn.cpp unrar/rdwrfn.cpp unrar/consio.cpp unrar/options.cpp unrar/ulinks.cpp unrar/errhnd.cpp unrar/rarvm.cpp unrar/rijndael.cpp unrar/getbits.cpp unrar/sha1.cpp unrar/extinfo.cpp unrar/extract.cpp unrar/volume.cpp unrar/list.cpp unrar/find.cpp unrar/unpack.cpp unrar/cmddata.cpp unrar/filestr.cpp unrar/scantree.cpp unrar/dll.cpp)

FILE(GLOB mmgrSRC mmgr/*.cpp)
add_library(mmgr ${mmgrSRC})
include_directories(mmgr)

FILE(GLOB tosserSRC tosser/*.cpp)
add_library(tosser ${tosserSRC})
include_directories(tosser)

FILE(GLOB bungleSRC bungle/*.cpp)
add_library(bungle ${bungleSRC})
include_directories(bungle)

FILE(GLOB eclipseSRC eclipse/*.cpp)
add_library(eclipse ${eclipseSRC})
include_directories(eclipse)

include_directories(ftgl/src ftgl/src/FTFont ftgl/src/FTGL ftgl/src/FTGlyph ftgl/src/FTLayout)
include_directories(freetype/include)
add_definitions(-DUSE_FTGL)
add_definitions(-DUSE_SDL)
add_library(gucci gucci/gucci.cpp gucci/image.cpp  gucci/gucci_sdl.cpp)
include_directories(gucci)

include_directories(unrar)
FILE(GLOB redshirtSRC redshirt/*.cpp)
add_library(redshirt ${redshirtSRC})
include_directories(redshirt)

include_directories(SDL_mixer/include)
include_directories(SDL/include)
FILE(GLOB soundgardenSRC soundgarden/*.cpp)
add_library(soundgarden ${soundgardenSRC})
include_directories(soundgarden)

FILE(GLOB vanbakelSRC vanbakel/*.cpp)
add_library(vanbakel ${vanbakelSRC})
include_directories(vanbakel)