project(tests_cppunit_3rdparty_proj CXX)

include_directories(${proj_INCLUDE_DIR})

add_3rdparty_cppunit_test(proj_tests 
  ex3rdparty ${proj_LIBRARIES})

if(WIN32)
  set(TESTS_PROJ_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${proj_BIN_DIR};${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_PROJ_PATH "${TESTS_PROJ_PATH}")
  #message(STATUS "TESTS_Proj_PATH: ${TESTS_PROJ_PATH}")
  set_tests_properties(proj_tests PROPERTIES ENVIRONMENT "${TESTS_PROJ_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(proj_tests)
endif()
