project(tests_performance_3rdparty_tbb CXX)

add_3rdparty_perf_test(tbb_perf_tests
  ex3rdparty 
  ${TBB_LIBRARIES})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_TBB_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_TBB_PATH "${TESTS_TBB_PATH}")
  message(STATUS "Tests_Tbb_Path: ${TESTS_TBB_PATH}")
  set_tests_properties(tbb_perf_tests PROPERTIES ENVIRONMENT "${TESTS_TBB_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  exai_install_test(tbb_perf_tests)
endif()
