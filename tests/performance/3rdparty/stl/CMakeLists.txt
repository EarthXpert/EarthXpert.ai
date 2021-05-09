project(tests_performance_3rdparty_stl CXX)

add_3rdparty_perf_test(stl_perf_tests
  ex3rdparty)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_STL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_STL_PATH "${TESTS_STL_PATH}")
  message(STATUS "Tests_Stl_Path: ${TESTS_STL_PATH}")
  set_tests_properties(stl_perf_tests PROPERTIES ENVIRONMENT "${TESTS_STL_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(stl_perf_tests)
endif()
