project(unit_tests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(unit_tests
        tests.cpp)

target_link_libraries(unit_tests gtest)
target_link_libraries(unit_tests functions)
target_link_libraries(unit_tests window)
target_link_libraries(unit_tests helpers)

add_test(NAME unit_tests COMMAND unit_tests)