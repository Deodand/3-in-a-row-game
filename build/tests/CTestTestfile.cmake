# CMake generated Testfile for 
# Source directory: C:/Users/Admin/Source/Repos/Deodand/3-in-a-row-game/tests
# Build directory: C:/Users/Admin/Source/Repos/Deodand/3-in-a-row-game/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(unit_tests "C:/Users/Admin/Source/Repos/Deodand/3-in-a-row-game/build/tests/Debug/unit_tests.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(unit_tests "C:/Users/Admin/Source/Repos/Deodand/3-in-a-row-game/build/tests/Release/unit_tests.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(unit_tests "C:/Users/Admin/Source/Repos/Deodand/3-in-a-row-game/build/tests/MinSizeRel/unit_tests.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(unit_tests "C:/Users/Admin/Source/Repos/Deodand/3-in-a-row-game/build/tests/RelWithDebInfo/unit_tests.exe")
else()
  add_test(unit_tests NOT_AVAILABLE)
endif()
