project(tests_performance_3rdparty_eigen CXX)

add_3rdparty_perf_test(eigen_perf_tests
  ex3rdparty
  Boost::boost)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_EIGEN_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_EIGEN_PATH "${TESTS_EIGEN_PATH}")
  message(STATUS "Tests_Eigen_Path: ${TESTS_EIGEN_PATH}")
  set_tests_properties(eigen_perf_tests PROPERTIES ENVIRONMENT "${TESTS_EIGEN_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(eigen_perf_tests)
endif()
