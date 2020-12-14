project(tests_performance_3rdparty_gpu CXX)

add_3rdparty_perf_test(gpu_perf_tests
  ex3rdparty
  ${CUDA_LIBRARIES} ${OpenCL_LIBRARIES})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_GPU_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_GPU_PATH "${TESTS_GPU_PATH}")
  message(STATUS "Tests_Gpu_Path: ${TESTS_GPU_PATH}")
  set_tests_properties(gpu_perf_tests PROPERTIES ENVIRONMENT "${TESTS_GPU_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(gpu_perf_tests)
endif()
