project(tests_cppunit_utils CXX)

function(add_utils_cppunit_test name)
  add_cppunit_test(${name}
    ex3rdparty core exutils 
    ${CERES_LIBRARY} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${LASLIB_LIBRARY} ${OpenCV_LIBS})
endfunction()

add_utils_cppunit_test(delaunay_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_UTILS_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;$<TARGET_FILE_DIR:exutils>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${geotrans_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_UTILS_PATH "${TESTS_UTILS_PATH}")
  message(STATUS "Tests_Utils_Path: ${TESTS_UTILS_PATH}")
  set_tests_properties(delaunay_tests 
    PROPERTIES ENVIRONMENT "${TESTS_UTILS_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(delaunay_tests)
endif()
