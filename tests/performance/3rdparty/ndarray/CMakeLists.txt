project(tests_performance_3rdparty_ndarray CXX)

add_3rdparty_perf_test(ndarray_perf_tests
  ex3rdparty
  Boost::boost Boost::numpy Boost::python)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_NDARRAY_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_NDARRAY_PATH "${TESTS_NDARRAY_PATH}")
  message(STATUS "Tests_Ndarray_Path: ${TESTS_NDARRAY_PATH}")
  set_tests_properties(ndarray_perf_tests PROPERTIES ENVIRONMENT "${TESTS_NDARRAY_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(ndarray_perf_tests)
endif()
