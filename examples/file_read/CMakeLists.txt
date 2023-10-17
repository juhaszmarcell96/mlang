add_executable(
    file_read
    main.cpp
)

target_link_libraries(
    file_read
    PUBLIC script_static
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wpa_supplicant.conf ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)