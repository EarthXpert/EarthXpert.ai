project(tests_cppunit_3rdparty_geotrans CXX)

include_directories(${geotrans_INCLUDE_DIR})

add_3rdparty_cppunit_test(geotrans_tests 
  ex3rdparty ${geotrans_LIBRARIES})

if(WIN32)
  set(TESTS_GEOTRANS_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${geotrans_BIN_DIR};${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_GEOTRANS_PATH "${TESTS_GEOTRANS_PATH}")
  #message(STATUS "TESTS_Geotrans_PATH: ${TESTS_GEOTRANS_PATH}")
  set_tests_properties(geotrans_tests PROPERTIES ENVIRONMENT "${TESTS_GEOTRANS_PATH}")
endif()

if(EXAI_INSTALL_TESTS_3RDPARTY)
  exai_install_test(geotrans_tests)
endif()
