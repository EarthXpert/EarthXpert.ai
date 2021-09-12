project(tests_cppunit_colorbal CXX)

function(add_colorbal_cppunit_test name)
  add_cppunit_test(${name} 
    ex3rdparty core colorbal dataio 
    ${GDAL_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_colorbal_cppunit_test(colorbal_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_COLORBAL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_COLORBAL_PATH "${TESTS_COLORBAL_PATH}")
  message(STATUS "Tests_Colorbal_Path: ${TESTS_COLORBAL_PATH}")
  set_tests_properties(colorbal_tests 
    PROPERTIES ENVIRONMENT "${TESTS_COLORBAL_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(colorbal_tests)
endif()
