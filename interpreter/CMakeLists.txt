add_executable(
    mlang
    main.cpp
)

target_link_libraries(
    mlang
    PUBLIC script_static
)