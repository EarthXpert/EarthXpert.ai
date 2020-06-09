project(tests_performance_io CXX)

add_performance_test(rasterreader_perf_tests 
  ex3rdparty core dataio 
  ${GDAL_LIBRARY} ${OPENCV_LIBS})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_IO_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OPENCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_IO_PATH "${TESTS_IO_PATH}")
  message(STATUS "Tests_Io_Path: ${TESTS_IO_PATH}")
  set_tests_properties(rasterreader_perf_tests
    PROPERTIES ENVIRONMENT "${TESTS_IO_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(rasterreader_perf_tests)
endif()
