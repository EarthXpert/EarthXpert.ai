project(tests_cppunit_core CXX)

function(add_core_cppunit_test name)
  add_cppunit_test(${name}
    ex3rdparty core 
    ${CERES_LIBRARY} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${LASLIB_LIBRARY} ${OpenCV_LIBS})
endfunction()

add_core_cppunit_test(coordinatesystem_tests)
add_core_cppunit_test(coordinatetransform_tests)
add_core_cppunit_test(earthellipsoid_tests)
add_core_cppunit_test(graphfit_tests)
add_core_cppunit_test(pointcloud_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_CORE_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;$<TARGET_FILE_DIR:core>;\
$<TARGET_FILE_DIR:dataio>;$<TARGET_FILE_DIR:epipolar>;$<TARGET_FILE_DIR:epipolarmodel>;\
$<TARGET_FILE_DIR:marshaller>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${geotrans_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_CORE_PATH "${TESTS_CORE_PATH}")
  message(STATUS "Tests_Core_Path: ${TESTS_CORE_PATH}")
  set_tests_properties(coordinatesystem_tests coordinatetransform_tests earthellipsoid_tests 
    graphfit_tests pointcloud_tests 
    PROPERTIES ENVIRONMENT "${TESTS_CORE_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_CPPUNIT)
  earthxpert_install_test(coordinatesystem_tests)
  earthxpert_install_test(coordinatetransform_tests)
  earthxpert_install_test(earthellipsoid_tests)
  earthxpert_install_test(graphfit_tests)
  earthxpert_install_test(pointcloud_tests)
endif()
