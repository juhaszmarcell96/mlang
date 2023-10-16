add_executable(
    simple
    main.cpp
)

target_link_libraries(
    simple
    PUBLIC script_static
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)