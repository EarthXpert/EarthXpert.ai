project(tests_cppunit_3rdparty_taskflow CXX)

add_3rdparty_cppunit_test(taskflow_tests
  ex3rdparty)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_TASKFLOW_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_TASKFLOW_PATH "${TESTS_TASKFLOW_PATH}")
  message(STATUS "Tests_Taskflow_Path: ${TESTS_TASKFLOW_PATH}")
  set_tests_properties(taskflow_tests PROPERTIES ENVIRONMENT "${TESTS_TASKFLOW_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  exai_install_test(taskflow_tests)
endif()
