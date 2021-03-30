project(tests_performance_sensormodel CXX)

function(add_sensormodel_perf_test name) 
  add_performance_test(${name}
    ex3rdparty core sensormodel
    ${CERES_LIBRARY} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_sensormodel_perf_test(rationalmodel_perf_tests)
add_sensormodel_perf_test(stereomodel_perf_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_SENSORMODEL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${CERES_BIN_DIR};${GDAL_BIN_DIR};${geotrans_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_SENSORMODEL_PATH "${TESTS_SENSORMODEL_PATH}")
  message(STATUS "Tests_Sensormodel_Path: ${TESTS_SENSORMODEL_PATH}")
  set_tests_properties(rationalmodel_perf_tests stereomodel_perf_tests
    PROPERTIES ENVIRONMENT "${TESTS_SENSORMODEL_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  exai_install_test(rationalmodel_perf_tests)  #CubeS2G #CalcJocobianA, CalcJacobianNc
  exai_install_test(stereomodel_perf_tests)
endif()
