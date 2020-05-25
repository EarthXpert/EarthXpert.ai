project(tests_cppunit_marshaller CXX)

add_cppunit_test(marshaller_tests 
  core epipolarmodel marshaller sensormodel 
  ${GDAL_LIBRARY})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TESTS_MARSHALLER_PATH "PATH=$<TARGET_FILE_DIR:ex3rd_party>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OPENCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_MARSHALLER_PATH "${TESTS_MARSHALLER_PATH}")
  message(STATUS "Tests_Mashaller_Path: ${TESTS_MARSHALLER_PATH}")
  set_tests_properties(marshaller_tests
    PROPERTIES ENVIRONMENT "${TESTS_MARSHALLER_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(marshaller_tests)
endif()
