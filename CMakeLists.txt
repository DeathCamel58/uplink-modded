cmake_minimum_required (VERSION 3.10)

# Use vcpkg libraries
if(WIN32)
    include(./uplink/lib/vcpkg/scripts/buildsystems/vcpkg.cmake)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/vcpkg/scripts/buildsystems/vcpkg.cmake
            CACHE STRING "Vcpkg toolchain file")
endif()

project (uplink)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(uplink)