# Locate GTest
find_package (GTest REQUIRED)
include_directories (${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable (
    tests
    main.cpp
    object_test.cpp
    #value_test.cpp
    tokenizer_test.cpp
    #ast_test.cpp
    #parser_test.cpp
    script_test.cpp
    error_test.cpp
    while_loop_test.cpp
    function_test.cpp
    indexing_test.cpp
    custom_class_test.cpp
    custom_func_test.cpp
)
target_link_libraries (tests ${GTEST_LIBRARIES} pthread script_static)