add_executable(
    basic
    main.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/script.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})