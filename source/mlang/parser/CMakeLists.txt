include(CMakePrintHelpers)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    parser_obj OBJECT
    parser.cpp
)

target_include_directories(
    parser_obj INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(
    parser_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

set(
    PARSER_INCLUDE_FILES
    mlang/parser/parser.hpp
)

set_target_properties(
    parser_obj PROPERTIES
    PUBLIC_HEADER "${PARSER_INCLUDE_FILES}"
    POSITION_INDEPENDENT_CODE 1
)

add_library(parser_shared SHARED)
target_link_libraries(
    parser_shared
    PUBLIC parser_obj ast_obj object_obj
)

add_library(parser_static STATIC)
target_link_libraries(
    parser_static
    PUBLIC parser_obj ast_obj object_obj
)

