cmake_minimum_required(VERSION 3.5)

project(ExampleProj)

enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include_directories(src/functions)
add_subdirectory(src)
add_subdirectory(lib/googletest-1.8.x)
add_subdirectory(tests)