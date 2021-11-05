project(tests_cppunit_3rdparty_gts CXX)

add_3rdparty_cppunit_test(gts_tests
  ex3rdparty
  ${GTS_LIBRARIES})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_GTS_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_GTS_PATH "${TESTS_GTS_PATH}")
  message(STATUS "Tests_Gts_Path: ${TESTS_GTS_PATH}")
  set_tests_properties(gts_tests PROPERTIES ENVIRONMENT "${TESTS_GTS_PATH}")
endif()

if(EXAI_INSTALL_TESTS_3RDPARTY)
  exai_install_test(gts_tests)
endif()
