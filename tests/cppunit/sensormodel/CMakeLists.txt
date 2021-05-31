project(tests_cppunit_sensormodel CXX)

function(add_sensormodel_cppunit_test name)
  add_cppunit_test(${name} 
    core marshaller sensormodel testutils
    ${GDAL_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_sensormodel_cppunit_test(framemodel_tests)
add_sensormodel_cppunit_test(rationalmodel_tests)
add_sensormodel_cppunit_test(stereomodel_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_SENSORMODEL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_SENSORMODEL_PATH "${TESTS_SENSORMODEL_PATH}")
  message(STATUS "Tests_Sensormodel_Path: ${TESTS_SENSORMODEL_PATH}")
  set_tests_properties(framemodel_tests rationalmodel_tests stereomodel_tests
    PROPERTIES ENVIRONMENT "${TESTS_SENSORMODEL_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(framemodel_tests)
  exai_install_test(rationalmodel_tests)
  exai_install_test(stereomodel_tests)
endif()
