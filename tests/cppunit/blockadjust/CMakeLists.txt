project(tests_cppunit_bba CXX)

add_cppunit_test(adsbba_tests 
  blockadjust core sensormodel
  ${GDAL_LIBRARY} ${OPENCV_LIBS})
add_cppunit_test(framebba_tests 
  blockadjust core sensormodel
  ${GDAL_LIBRARY} ${OPENCV_LIBS})
add_cppunit_test(rationalbba_tests 
  blockadjust core sensormodel
  ${GDAL_LIBRARY} ${OPENCV_LIBS})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_BBA_PATH "PATH=$<TARGET_FILE_DIR:ex3rd_party>;$<TARGET_FILE_DIR:blockadjust>;\
$<TARGET_FILE_DIR:core>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OPENCV_BIN_DIR};${OPENNI_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_COREBBA_PATH "${TESTS_BBA_PATH}")
  message(STATUS "Tests_Bba_Path: ${TESTS_BBA_PATH}")
  set_tests_properties(framebba_tests PROPERTIES ENVIRONMENT "${TESTS_BBA_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(adsbba_tests)
  earthxpert_install_test(framebba_tests)
  earthxpert_install_test(rationalbba_tests)
endif()
