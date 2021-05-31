project(tests_cppunit_3rdparty_gsl CXX)

add_3rdparty_cppunit_test(gsl_tests 
  ex3rdparty
  Boost::boost ${gsl_LIBRARIES})

if(WIN32)
  set(TESTS_GSL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_GSL_PATH "${TESTS_GSL_PATH}")
  #message(STATUS "Tests_Gsl_Path: ${TESTS_GSL_PATH}")
  set_tests_properties(gsl_tests PROPERTIES ENVIRONMENT "${TESTS_GSL_PATH}")
endif()

if(EXAI_INSTALL_TESTS_3RDPARTY)
  exai_install_test(gsl_tests)
endif()
