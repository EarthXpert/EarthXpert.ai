project(tests_cppunit_ortho CXX)

function(add_ortho_cppunit_test name)
  add_cppunit_test(${name} 
    ex3rdparty core dataio ortho
    ${GDAL_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_ortho_cppunit_test(ortho_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_ORTHO_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_ORTHO_PATH "${TESTS_ORTHO_PATH}")
  message(STATUS "Tests_Ortho_Path: ${TESTS_ORTHO_PATH}")
  set_tests_properties(ortho_tests 
    PROPERTIES ENVIRONMENT "${TESTS_ORTHO_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(ortho_tests)
endif()
