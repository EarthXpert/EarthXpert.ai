project(tests_cppunit_3rdparty_ceres CXX)

include_directories(${CERES_INCLUDE_DIRS})

add_3rdparty_cppunit_test(ceres_tests 
  ex3rdparty 
  Boost::boost ${CERES_LIBRARY})

if(WIN32)
  set(TESTS_CERES_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${CERES_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_CERES_PATH "${TESTS_CERES_PATH}")
  #message(STATUS "Tests_Ceres_Path: ${TESTS_CERES_PATH}")
  set_tests_properties(ceres_tests PROPERTIES ENVIRONMENT "${TESTS_CERES_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  exai_install_test(ceres_tests)
endif()
