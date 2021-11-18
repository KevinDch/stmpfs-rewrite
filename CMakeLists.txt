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
set(SHA256SUM_SRC_FILES "")
set(SHA256SUM_SRC_FILE_NO_PREFIX sha256sum.cpp)
generate_ext_lib_files(${SHA256SUM_SRC_FILE_NO_PREFIX}
        sha256sum SHA256SUM_SRC_FILES)
add_ext_lib(sha256sum ${SHA256SUM_SRC_FILES})

# STEMPFS Static Library
add_library(stmpfs STATIC
        src/stmpfs_error.cpp    src/include/stmpfs_error.h
        src/debug.cpp           src/include/debug.h
        src/buffer.cpp          src/include/buffer.h
        src/pathname_t.cpp      src/include/pathname_t.h
        src/inode.cpp           src/include/inode.h
        )
target_include_directories(stmpfs PUBLIC src/include)
target_link_libraries(stmpfs PUBLIC ${EXTERNAL_LIBRARIES})

# Unit tests
if ("X${CMAKE_BUILD_TYPE}" STREQUAL "XDebug")
    stmpfs_add_test(error       "Error test")
    stmpfs_add_test(debug       "Debug test")
    stmpfs_add_test(buffer      "Buffer test")
    stmpfs_add_test(pathname    "Pathname test")
    stmpfs_add_test(inode       "Inode test")
endif()

