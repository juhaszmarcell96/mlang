add_executable(
    parallel
    main.cpp
)

target_link_libraries(
    parallel
    PUBLIC script_static
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script_1.mlang ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script_2.mlang ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/values.conf ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)