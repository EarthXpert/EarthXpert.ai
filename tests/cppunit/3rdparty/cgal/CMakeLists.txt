project(tests_cppunit_3rdparty_cgal CXX)

include_directories(${CGAL_INCLUDE_DIRS})

add_3rdparty_cppunit_test(cgal_tests 
  ex3rdparty 
  ${CGAL_LIBRARIES})

if(WIN32)
  set(TESTS_CGAL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${CGAL_BIN_DIR};${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_CGAL_PATH "${TESTS_CGAL_PATH}")
  #message(STATUS "Tests_Cgal_Path: ${TESTS_CGAL_PATH}")
  set_tests_properties(cgal_tests PROPERTIES ENVIRONMENT "${TESTS_CGAL_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  exai_install_test(cgal_tests)
endif()
