project(tests_cppunit_3rdparty_matplotlib CXX)

add_3rdparty_cppunit_test(matplotlib_tests
  ex3rdparty
  ${PYTHON_LIBRARIES})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_MATPLOTLIB_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_MATPLOTLIB_PATH "${TESTS_MATPLOTLIB_PATH}")
  message(STATUS "Tests_Matplotlib_Path: ${TESTS_MATPLOTLIB_PATH}")
  set_tests_properties(matplotlib_tests PROPERTIES ENVIRONMENT "${TESTS_MATPLOTLIB_PATH}")
endif()

if(EXAI_INSTALL_TESTS_3RDPARTY)
  exai_install_test(matplotlib_tests)
endif()
