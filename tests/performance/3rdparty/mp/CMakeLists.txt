project(tests_performance_3rdparty_mp CXX)

add_3rdparty_perf_test(mp_perf_tests
  ex3rdparty
  Boost::boost ${MPFR4_LIBRARIES} ${MPIR_LIBRARIES})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_MP_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_MP_PATH "${TESTS_MP_PATH}")
  message(STATUS "Tests_Mp_Path: ${TESTS_MP_PATH}")
  set_tests_properties(mp_perf_tests PROPERTIES ENVIRONMENT "${TESTS_MP_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(mp_perf_tests)
endif()
