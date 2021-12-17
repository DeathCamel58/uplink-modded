project(unrar CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

message("Unrar Configuration:")

#
option(UNRAR_LARGEFILE_SOURCE "" ON)
if (UNRAR_LARGEFILE_SOURCE)
    add_definitions(-D_LARGEFILE_SOURCE)
endif ()
message("-- UNRAR_LARGEFILE_SOURCE: '${UNRAR_LARGEFILE_SOURCE}'")

#
option(UNRAR_RARDLL "" ON)
if (UNRAR_RARDLL)
    add_definitions(-DRARDLL)
endif ()
message("-- UNRAR_RARDLL: '${UNRAR_RARDLL}'")

#
SET(UNRAR_FILE_OFFSET_BITS "64" CACHE STRING "")
add_definitions(-D_FILE_OFFSET_BITS=${UNRAR_FILE_OFFSET_BITS})
message("-- UNRAR_FILE_OFFSET_BITS: '${UPLINK_VERSION}'")

FILE(GLOB unrarSRC *.cpp)
FILE(GLOB unrarHeaders *.hpp)
add_definitions(-O2)
add_library(unrar ${unrarHeaders} filestr.cpp recvol.cpp rs.cpp scantree.cpp rar.cpp strlist.cpp strfn.cpp pathfn.cpp int64.cpp savepos.cpp global.cpp file.cpp filefn.cpp filcreat.cpp archive.cpp arcread.cpp unicode.cpp system.cpp isnt.cpp crypt.cpp crc.cpp rawread.cpp encname.cpp resource.cpp match.cpp timefn.cpp rdwrfn.cpp consio.cpp options.cpp ulinks.cpp errhnd.cpp rarvm.cpp rijndael.cpp getbits.cpp sha1.cpp extinfo.cpp extract.cpp volume.cpp list.cpp find.cpp unpack.cpp cmddata.cpp filestr.cpp scantree.cpp dll.cpp)

set(UNRAR_INCLUDE_DIR ${PROJECT_SOURCE_DIR} CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)