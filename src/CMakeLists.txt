project(Example)

add_executable(main main.cpp)

add_subdirectory(functions)

target_link_libraries(main functions)