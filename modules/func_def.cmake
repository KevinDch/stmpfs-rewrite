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
