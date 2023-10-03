add_executable(
    basic
    main.cpp
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)