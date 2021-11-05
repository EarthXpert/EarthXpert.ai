project(tests_performance_3rdparty_simd CXX)

add_3rdparty_perf_test(simd_perf_tests
  ex3rdparty 
  ${nsimd_LIBRARIES} ${Simd_LIBRARIES})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_SIMD_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_SIMD_PATH "${TESTS_SIMD_PATH}")
  message(STATUS "Tests_Simd_Path: ${TESTS_SIMD_PATH}")
  set_tests_properties(simd_perf_tests PROPERTIES ENVIRONMENT "${TESTS_SIMD_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(simd_perf_tests)
endif()
