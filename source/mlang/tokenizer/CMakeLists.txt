include(CMakePrintHelpers)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    tokenizer_obj OBJECT
    code.cpp
    tokenizer.cpp
)

target_include_directories(
    tokenizer_obj INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(
    tokenizer_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

set(
    TOKENIZER_INCLUDE_FILES
    mlang/tokenizer/tokenizer.hpp
    mlang/tokenizer/code.hpp
)

set_target_properties(
    tokenizer_obj PROPERTIES
    PUBLIC_HEADER "${TOKENIZER_INCLUDE_FILES}"
    POSITION_INDEPENDENT_CODE 1
)

add_library(tokenizer_shared SHARED)
target_link_libraries(
    tokenizer_shared
    PUBLIC tokenizer_obj
)

add_library(tokenizer_static STATIC)
target_link_libraries(
    tokenizer_static
    PUBLIC tokenizer_obj
)

