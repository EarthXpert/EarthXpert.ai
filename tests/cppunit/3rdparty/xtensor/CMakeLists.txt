project(tests_cppunit_3rdparty_xtensor CXX)

add_3rdparty_cppunit_test(xtensor_tests
  ex3rdparty)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_XTENSOR_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_XTENSOR_PATH "${TESTS_XTENSOR_PATH}")
  message(STATUS "Tests_Xtensor_Path: ${TESTS_XTENSOR_PATH}")
  set_tests_properties(xtensor_tests PROPERTIES ENVIRONMENT "${TESTS_XTENSOR_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(xtensor_tests)
endif()
