project(tests_cppunit_epigen CXX)

function(add_epipolar_cppunit_test name)
  add_cppunit_test(${name} 
    ex3rdparty core dataio epipolar epipolarmodel marshaller sensormodel
    ${GDAL_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_epipolar_cppunit_test(epigen_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_EPIGEN_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_EPIGEN_PATH "${TESTS_EPIGEN_PATH}")
  message(STATUS "Tests_Epigen_Path: ${TESTS_EPIGEN_PATH}")
  set_tests_properties(epigen_tests 
    PROPERTIES ENVIRONMENT "${TESTS_EPIGEN_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(epigen_tests)
endif()
