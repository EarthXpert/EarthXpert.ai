project(tests_3rdparty_pdal CXX)

include_directories(${PDAL_INCLUDE_DIRS})

add_3rdparty_test(pdal_tests 
  ex3rdparty)

if(WIN32)
  set(TESTS_PDAL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${PDAL_BIN_DIR};${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_PDAL_PATH "${TESTS_PDAL_PATH}")
  #message(STATUS "Tests_Pdal_Path: ${TESTS_PDAL_PATH}")
  set_tests_properties(pdal_tests PROPERTIES ENVIRONMENT "${TESTS_PDAL_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(pdal_tests)
endif()
