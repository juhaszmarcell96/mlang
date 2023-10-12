include(CMakePrintHelpers)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    script_obj OBJECT
    token.cpp
    environment.cpp
    script.cpp
)

target_include_directories(
    script_obj INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(
    script_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

set(
    SCRIPT_INCLUDE_FILES
    mlang/script/token.hpp
    mlang/script/environment.hpp
    mlang/script/script.hpp
)

set_target_properties(
    script_obj PROPERTIES
    PUBLIC_HEADER "${SCRIPT_INCLUDE_FILES}"
    POSITION_INDEPENDENT_CODE 1
)

add_library(script_shared SHARED)
target_link_libraries(
    script_shared
    PUBLIC script_obj parser_obj tokenizer_obj ast_obj object_obj
)

add_library(script_static STATIC)
target_link_libraries(
    script_static
    PUBLIC script_obj parser_obj tokenizer_obj ast_obj object_obj
)

