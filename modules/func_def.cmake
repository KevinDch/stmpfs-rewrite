function(stmpfs_add_test TEST DESCRIPTION)
    set(TEST_NAME "UT_${TEST}")

    add_executable(${TEST_NAME} "tests/${TEST}_test.cpp")
    target_link_libraries(${TEST_NAME} PUBLIC stmpfs)
    target_include_directories(${TEST_NAME} PUBLIC src/include)

    add_test(NAME ${TEST_NAME}
            COMMAND ${TEST_NAME}
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    message("Unit test `${TEST_NAME}`\t(${DESCRIPTION}) enabled")
endfunction()

function(default_definition DEFINITION DEFAULT_VAL)
    # if definition not predefined
    if ("X${${DEFINITION}}" STREQUAL "X")
        add_compile_definitions("${DEFINITION}=${DEFAULT_VAL}")
    endif()
endfunction()

function(add_ext_lib LIBNAME SOURCE_FILE_VAL_NAME)
    message("External library `${LIBNAME}` added")
    add_library(${LIBNAME} STATIC ${SOURCE_FILE_VAL_NAME})
    set(EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} ${LIBNAME})
endfunction()

function(generate_ext_lib_files FILENAME FPREFIX RETURN_VAL_NAME)
    foreach(FILE ${FILENAME})
        set(${RETURN_VAL_NAME} src/ext_lib/${FPREFIX}/${FILE} PARENT_SCOPE)
    endforeach()
endfunction()
