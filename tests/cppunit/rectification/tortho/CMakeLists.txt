project(tests_cppunit_tortho CXX)

function(add_tortho_cppunit_test name)
  add_cppunit_test(${name} 
    ex3rdparty core dataio sensormodel tortho
    ${GDAL_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_tortho_cppunit_test(tortho_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_TORTHO_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_TORTHO_PATH "${TESTS_TORTHO_PATH}")
  message(STATUS "Tests_Tortho_Path: ${TESTS_TORTHO_PATH}")
  set_tests_properties(tortho_tests 
    PROPERTIES ENVIRONMENT "${TESTS_TORTHO_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(tortho_tests)
endif()
