project(tests_cppunit_dem CXX)

function(add_dem_cppunit_test name)
  add_cppunit_test(${name} 
    core dataio exdem sensormodel
    ${GDAL_LIBRARIES} ${OpenCV_LIBS})
endfunction()

add_dem_cppunit_test(dem_tests)
add_dem_cppunit_test(geoid_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_DEM_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
$<TARGET_FILE_DIR:core>;$<TARGET_FILE_DIR:exdem>;$<TARGET_FILE_DIR:sensormodel>;\
${GTEST_BIN_DIR};${BOOST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_DEM_PATH "${TESTS_DEM_PATH}")
  message(STATUS "Tests_Dem_Path: ${TESTS_DEM_PATH}")
  set_tests_properties(dem_tests geoid_tests  
    PROPERTIES ENVIRONMENT "${TESTS_DEM_PATH}")
endif()

if(EXAI_INSTALL_TESTS_CPPUNIT)
  exai_install_test(dem_tests)
  exai_install_test(geoid_tests)
endif()
