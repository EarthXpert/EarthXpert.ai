project(tests_cppunit_3rdparty_stl CXX)

add_3rdparty_cppunit_test(stl_tests)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_STL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_STL_PATH "${TESTS_STL_PATH}")
  message(STATUS "Tests_Stl_Path: ${TESTS_STL_PATH}")
  set_tests_properties(stl_tests PROPERTIES ENVIRONMENT "${TESTS_STL_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(stl_tests)
endif()
