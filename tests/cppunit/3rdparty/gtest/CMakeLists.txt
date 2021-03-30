project(tests_cppunit_3rdparty_gtest CXX)

include_directories(${GTEST_INCLUDE_DIR})

add_3rdparty_cppunit_test(gtest_tests
  ex3rdparty)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TESTS_GTEST_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_GTEST_PATH "${TESTS_GTEST_PATH}")
  message(STATUS "Tests_Gtest_Path: ${TESTS_GTEST_PATH}")
  set_tests_properties(gtest_tests PROPERTIES ENVIRONMENT "${TESTS_GTEST_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  exai_install_test(gtest_tests)
endif()
