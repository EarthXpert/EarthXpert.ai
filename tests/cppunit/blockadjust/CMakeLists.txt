project(tests_cppunit_bba CXX)

function(add_bba_cppunit_test name)
  add_cppunit_test(${name} 
    blockadjust core sensormodel
    ${GDAL_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_bba_cppunit_test(adsbba_tests)
add_bba_cppunit_test(framebba_tests)
add_bba_cppunit_test(rationalbba_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_BBA_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:blockadjust>;\
$<TARGET_FILE_DIR:core>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_COREBBA_PATH "${TESTS_BBA_PATH}")
  message(STATUS "Tests_Bba_Path: ${TESTS_BBA_PATH}")
  set_tests_properties(adsbba_tests framebba_tests rationalbba_tests 
    PROPERTIES ENVIRONMENT "${TESTS_BBA_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(adsbba_tests)
  exai_install_test(framebba_tests)
  exai_install_test(rationalbba_tests)
endif()
