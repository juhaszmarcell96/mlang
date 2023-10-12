add_executable(
    basic
    main.cpp
)

target_link_libraries(
    basic
    PUBLIC parser_static tokenizer_static
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)