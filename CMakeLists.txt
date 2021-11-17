cmake_minimum_required(VERSION 3.20)
project(stmpfs_rewrite)

set(EXTERNAL_LIBRARIES "")

set(CMAKE_CXX_STANDARD 20)

include(modules/func_def.cmake)

if ("X${CMAKE_BUILD_TYPE}" STREQUAL "XDebug")
    add_compile_definitions(CMAKE_BUILD_DEBUG=1)
endif()

# default definitions
default_definition(STMPFS_PREFIX "\"(stmpfs) \"")

# add external library
#set(ADDR2LINE_SRC_FILES "")
#set(ADDR2LINE_SRC_FILE_NO_PREFIX addr2line.cpp)
#generate_ext_lib_files(${ADDR2LINE_SRC_FILE_NO_PREFIX}
#        addr2line ADDR2LINE_SRC_FILES)
#add_ext_lib(addr2line ${ADDR2LINE_SRC_FILES})

# STEMPFS Static Library
add_library(stmpfs STATIC
        src/stmpfs_error.cpp    src/include/stmpfs_error.h
        src/debug.cpp           src/include/debug.h
        )
target_include_directories(stmpfs PUBLIC src/include)

# Unit tests
if ("X${CMAKE_BUILD_TYPE}" STREQUAL "XDebug")
    stmpfs_add_test(error "Error test")
    stmpfs_add_test(debug "Debug test")
endif()

