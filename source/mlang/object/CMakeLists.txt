include(CMakePrintHelpers)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    object_obj OBJECT
    internal_object.cpp
    object.cpp
    none.cpp
    boolean.cpp
    int.cpp
    float.cpp
    string.cpp
    array.cpp
)

target_include_directories(
    object_obj INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(
    object_obj PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

set(
    OBJECT_INCLUDE_FILES
    mlang/object/internal_object.hpp
    mlang/object/object.hpp
    mlang/object/none.hpp
    mlang/object/boolean.hpp
    mlang/object/int.hpp
    mlang/object/float.hpp
    mlang/object/string.hpp
    mlang/object/array.hpp
    mlang/object/assert.hpp
)

set_target_properties(
    object_obj PROPERTIES
    PUBLIC_HEADER "${OBJECT_INCLUDE_FILES}"
    POSITION_INDEPENDENT_CODE 1
)

add_library(object_shared SHARED)
target_link_libraries(
    object_shared
    PUBLIC object_obj
)

add_library(object_static STATIC)
target_link_libraries(
    object_static
    PUBLIC object_obj
)