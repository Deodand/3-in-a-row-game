project(Example)

add_executable(main main.cpp)

add_subdirectory(functions)

target_link_libraries(main functions)

SET_TARGET_PROPERTIES(main PROPERTIES LINK_FLAGS    "/SUBSYSTEM:WINDOWS")