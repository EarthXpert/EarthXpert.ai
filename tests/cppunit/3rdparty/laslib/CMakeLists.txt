project(tests_cppunit_3rdparty_laslib CXX)

include_directories(${LASLIB_INCLUDE_DIR})

add_3rdparty_cppunit_test(laslib_tests 
  ex3rdparty 
  ${LASLIB_LIBRARY})
  
if(WIN32)
  set(TESTS_LASLIB_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${LASLIB_BIN_DIR};${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_LASLIB_PATH "${TESTS_LASLIB_PATH}")
  #message(STATUS "Tests_Laslib_Path: ${TESTS_LASLIB_PATH}")
  set_tests_properties(laslib_tests PROPERTIES ENVIRONMENT "${TESTS_LASLIB_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(laslib_tests)
endif()
