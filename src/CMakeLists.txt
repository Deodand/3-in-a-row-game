project(Example)

add_executable(main main.cpp)

add_subdirectory(functions)
add_subdirectory(window)
add_subdirectory(helpers)

target_link_libraries(main functions)
target_link_libraries(main window)
target_link_libraries(main helpers)


SET_TARGET_PROPERTIES(main PROPERTIES LINK_FLAGS    "/SUBSYSTEM:WINDOWS")

