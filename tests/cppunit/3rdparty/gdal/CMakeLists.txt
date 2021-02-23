project(tests_cppunit_3rdparty_gdal CXX)

add_3rdparty_cppunit_test(gdal_tests 
  ex3rdparty ${GDAL_LIBRARIES})

if(WIN32)
  set(TESTS_GDAL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${GDAL_BIN_DIR};${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_GDAL_PATH "${TESTS_GDAL_PATH}")
  #message(STATUS "Tests_Gdal_Path: ${TESTS_GDAL_PATH}")
  set_tests_properties(gdal_tests PROPERTIES ENVIRONMENT "${TESTS_GDAL_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(gdal_tests)
endif()
