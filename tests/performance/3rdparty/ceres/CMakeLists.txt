project(tests_performance_3rdparty_ceres CXX)

add_3rdparty_perf_test(ceres_perf_tests
  ex3rdparty 
  ${CERES_LIBRARY})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_CERES_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_CERES_PATH "${TESTS_CERES_PATH}")
  message(STATUS "Tests_Ceres_Path: ${TESTS_CERES_PATH}")
  set_tests_properties(ceres_perf_tests PROPERTIES ENVIRONMENT "${TESTS_CERES_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  exai_install_test(ceres_perf_tests)
endif()
