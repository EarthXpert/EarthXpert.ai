project(tests_cppunit_3rdparty_interpolate CXX)

add_3rdparty_cppunit_test(interpolate_tests 
  ex3rdparty )

if(WIN32)
  set(TESTS_INTERPOLATE_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${proj_BIN_DIR};${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_INTERPOLATE_PATH "${TESTS_INTERPOLATE_PATH}")
  #message(STATUS "TESTS_Interpolate_PATH: ${TESTS_INTERPOLATE_PATH}")
  set_tests_properties(interpolate_tests PROPERTIES ENVIRONMENT "${TESTS_INTERPOLATE_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(interpolate_tests)
endif()
