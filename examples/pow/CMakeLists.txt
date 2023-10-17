add_executable(
    pow
    main.cpp
)

target_link_libraries(
    pow
    PUBLIC script_static
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script.mlang ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)