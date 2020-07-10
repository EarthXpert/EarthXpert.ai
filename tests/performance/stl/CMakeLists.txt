project(tests_performance_stl CXX)

add_performance_test(stl_perf_tests)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_STL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_STL_PATH "${TESTS_STL_PATH}")
  message(STATUS "Tests_Stl_Path: ${TESTS_STL_PATH}")
  set_tests_properties(stl_perf_tests PROPERTIES ENVIRONMENT "${TESTS_STL_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(stl_perf_tests)
endif()
