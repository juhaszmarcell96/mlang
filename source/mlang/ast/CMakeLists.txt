include(CMakePrintHelpers)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    ast_obj OBJECT
    array_node.cpp
    assignment.cpp
    binary_operations.cpp
    block_node.cpp
    break_node.cpp
    comparison.cpp
    continue_node.cpp
    declaration.cpp
    exit_node.cpp
    for_node.cpp
    func_call_node.cpp
    func_decl_node.cpp
    if_node.cpp
    logic_operations.cpp
    main_node.cpp
    member_access.cpp
    member_function.cpp
    print_node.cpp
    return_node.cpp
    subscript_node.cpp
    unary_operations.cpp
    value_node.cpp
    variable_node.cpp
    while_node.cpp
    constructor_node.cpp
)

target_include_directories(
    ast_obj INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(
    ast_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

set(
    AST_INCLUDE_FILES
    mlang/ast/array_node.hpp
    mlang/ast/assignment.hpp
    mlang/ast/binary_operations.hpp
    mlang/ast/block_node.hpp
    mlang/ast/break_node.hpp
    mlang/ast/comparison.hpp
    mlang/ast/continue_node.hpp
    mlang/ast/declaration.hpp
    mlang/ast/exception.hpp
    mlang/ast/exit_node.hpp
    mlang/ast/for_node.hpp
    mlang/ast/func_call_node.hpp
    mlang/ast/func_decl_node.hpp
    mlang/ast/if_node.hpp
    mlang/ast/logic_operations.hpp
    mlang/ast/main_node.hpp
    mlang/ast/member_access.hpp
    mlang/ast/member_function.hpp
    mlang/ast/node.hpp
    mlang/ast/print_node.hpp
    mlang/ast/return_node.hpp
    mlang/ast/subscript_node.hpp
    mlang/ast/unary_operations.hpp
    mlang/ast/value_node.hpp
    mlang/ast/variable_node.hpp
    mlang/ast/while_node.hpp
    mlang/ast/constructor_node.hpp
)

set_target_properties(
    ast_obj PROPERTIES
    PUBLIC_HEADER "${AST_INCLUDE_FILES}"
    POSITION_INDEPENDENT_CODE 1
)

add_library(ast_shared SHARED)
target_link_libraries(
    ast_shared
    PUBLIC ast_obj object_obj
)

add_library(ast_static STATIC)
target_link_libraries(
    ast_static
    PUBLIC ast_obj object_obj
)

