project(tests_performance_core CXX)

function(add_core_perf_test name)
  add_performance_test(${name} 
    ex3rdparty core 
    ${CERES_LIBRARY} ${GDAL_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_core_perf_test(graphfit_perf_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_CORE_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${CERES_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_CORE_PATH "${TESTS_CORE_PATH}")
  message(STATUS "Tests_Core_Path: ${TESTS_CORE_PATH}")
  set_tests_properties(graphfit_perf_tests
    PROPERTIES ENVIRONMENT "${TESTS_CORE_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(graphfit_perf_tests)
endif()
