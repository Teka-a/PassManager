# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/PassManager_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/PassManager_autogen.dir/ParseCache.txt"
  "PassManager_autogen"
  "_deps/googletest-build/googlemock/CMakeFiles/gmock_autogen.dir/AutogenUsed.txt"
  "_deps/googletest-build/googlemock/CMakeFiles/gmock_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googlemock/CMakeFiles/gmock_main_autogen.dir/AutogenUsed.txt"
  "_deps/googletest-build/googlemock/CMakeFiles/gmock_main_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googlemock/gmock_autogen"
  "_deps/googletest-build/googlemock/gmock_main_autogen"
  "_deps/googletest-build/googletest/CMakeFiles/gtest_autogen.dir/AutogenUsed.txt"
  "_deps/googletest-build/googletest/CMakeFiles/gtest_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googletest/CMakeFiles/gtest_main_autogen.dir/AutogenUsed.txt"
  "_deps/googletest-build/googletest/CMakeFiles/gtest_main_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googletest/gtest_autogen"
  "_deps/googletest-build/googletest/gtest_main_autogen"
  "launcher/CMakeFiles/startDebugger_autogen.dir/AutogenUsed.txt"
  "launcher/CMakeFiles/startDebugger_autogen.dir/ParseCache.txt"
  "launcher/startDebugger_autogen"
  "test/CMakeFiles/file_autogen.dir/AutogenUsed.txt"
  "test/CMakeFiles/file_autogen.dir/ParseCache.txt"
  "test/file_autogen"
  )
endif()
