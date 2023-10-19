add_executable(
    localtime
    main.cpp
)

target_link_libraries(
    localtime
    PUBLIC script_static
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script.mlang ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/localtime.tz ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)