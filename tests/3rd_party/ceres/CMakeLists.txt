project(tests_3rd_party_ceres CXX)

include_directories(${CERES_INCLUDE_DIRS})

add_3rd_party_test(ceres_tests 
  ex3rd_party 
  ${CERES_LIBRARY})

if(WIN32)
  set(TESTS_CERES_PATH "PATH=$<TARGET_FILE_DIR:ex3rd_party>;${GTEST_BIN_DIR};${CERES_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_CERES_PATH "${TESTS_CERES_PATH}")
  #message(STATUS "Tests_Ceres_Path: ${TESTS_CERES_PATH}")
  set_tests_properties(ceres_tests PROPERTIES ENVIRONMENT "${TESTS_CERES_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(ceres_tests)
endif()
