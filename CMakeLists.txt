cmake_minimum_required(VERSION 3.20)
project(stmpfs_rewrite)

set(CMAKE_CXX_STANDARD 20)

include(modules/func_def.cmake)

# default definitions

default_definition(STMPFS_PREFIX "\"(stmpfs) \"")

add_library(stmpfs STATIC
        src/stmpfs_error.cpp    src/include/stmpfs_error.h
                                src/include/debug.h
        )
target_include_directories(stmpfs PUBLIC src/include)

stmpfs_add_test(error "Error test")
