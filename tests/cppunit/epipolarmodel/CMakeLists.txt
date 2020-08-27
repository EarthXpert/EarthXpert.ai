project(tests_cppunit_epimodel CXX)

add_cppunit_test(epipolarmodel_tests 
  ex3rdparty core dataio epipolarmodel marshaller sensormodel
  ${GDAL_LIBRARY} ${OpenCV_LIBS})
add_cppunit_test(epipolarstereofactory_tests 
  ex3rdparty core dataio epipolarmodel marshaller sensormodel
  ${GDAL_LIBRARY} ${OpenCV_LIBS})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_EPIMODEL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_EPIMODEL_PATH "${TESTS_EPIMODEL_PATH}")
  message(STATUS "Tests_Epimodel_Path: ${TESTS_EPIMODEL_PATH}")
  set_tests_properties(epipolarmodel_tests 
    PROPERTIES ENVIRONMENT "${TESTS_EPIMODEL_PATH}")
  set_tests_properties(epipolarstereofactory_tests 
    PROPERTIES ENVIRONMENT "${TESTS_EPIMODEL_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(epipolarmodel_tests)
  earthxpert_install_test(epipolarstereofactory_tests)
endif()
