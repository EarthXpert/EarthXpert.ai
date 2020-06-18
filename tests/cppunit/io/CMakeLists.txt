project(tests_cppunit_io CXX)

add_cppunit_test(pointcloudreader_tests 
  ex3rdparty core dataio 
  ${GDAL_LIBRARY} ${LASLIB_LIBRARY})
add_cppunit_test(pointcloudwriter_tests 
  ex3rdparty core dataio 
  ${GDAL_LIBRARY} ${LASLIB_LIBRARY})
add_cppunit_test(rasterreader_tests 
  ex3rdparty core dataio 
  ${GDAL_LIBRARY} ${OpenCV_LIBS})
add_cppunit_test(rasterwriter_tests 
  ex3rdparty core dataio 
  ${GDAL_LIBRARY} ${OpenCV_LIBS})
add_cppunit_test(vectorreader_tests 
  ex3rdparty core dataio 
  ${GDAL_LIBRARY})
add_cppunit_test(vectorwriter_tests 
  ex3rdparty core dataio 
  ${GDAL_LIBRARY})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_IO_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_IO_PATH "${TESTS_IO_PATH}")
  message(STATUS "Tests_Io_Path: ${TESTS_IO_PATH}")
  set_tests_properties(pointcloudreader_tests pointcloudwriter_tests 
    rasterreader_tests rasterwriter_tests vectorreader_tests vectorwriter_tests
    PROPERTIES ENVIRONMENT "${TESTS_IO_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(pointcloudreader_tests)
  earthxpert_install_test(pointcloudwriter_tests)
  earthxpert_install_test(rasterreader_tests)
  earthxpert_install_test(rasterwriter_tests)
  earthxpert_install_test(vectorreader_tests)
  earthxpert_install_test(vectorwriter_tests)
endif()
