project(tests_cppunit_3rdparty_vld CXX)

add_3rdparty_cppunit_test(vld_tests
  ex3rdparty)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_VLD_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_VLD_PATH "${TESTS_VLD_PATH}")
  message(STATUS "Tests_Vld_Path: ${TESTS_VLD_PATH}")
  set_tests_properties(vld_tests PROPERTIES ENVIRONMENT "${TESTS_VLD_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(vld_tests)
endif()
