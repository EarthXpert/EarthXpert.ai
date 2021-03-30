project(tests_cppunit_3rdparty_eigen CXX)

add_3rdparty_cppunit_test(eigen_tests 
  ex3rdparty
  Boost::boost)

if(WIN32)
  set(TESTS_EIGEN_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_EIGEN_PATH "${TESTS_EIGEN_PATH}")
  #message(STATUS "Tests_Eigen_Path: ${TESTS_EIGEN_PATH}")
  set_tests_properties(eigen_tests PROPERTIES ENVIRONMENT "${TESTS_EIGEN_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  exai_install_test(eigen_tests)
endif()
