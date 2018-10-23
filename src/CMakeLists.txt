project(Example)

add_executable(main main.cpp)

add_subdirectory(functions)
add_subdirectory(window)

target_link_libraries(main functions)
target_link_libraries(main window)