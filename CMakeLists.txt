cmake_minimum_required(VERSION 3.12)

project(ExampleProj)

include_directories(src/functions)
include_directories(src/window)
include_directories(src/helpers)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()
add_subdirectory(src)
add_subdirectory(lib/googletest-1.8.x)
add_subdirectory(tests)
