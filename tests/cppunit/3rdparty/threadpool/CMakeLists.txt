project(tests_cppunit_3rdparty_threadpool CXX)

add_3rdparty_cppunit_test(threadpool_tests
  ex3rdparty)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_THREADPOOL_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_THREADPOOL_PATH "${TESTS_THREADPOOL_PATH}")
  message(STATUS "Tests_Threadpool_Path: ${TESTS_THREADPOOL_PATH}")
  set_tests_properties(threadpool_tests PROPERTIES ENVIRONMENT "${TESTS_THREADPOOL_PATH}")
endif()

if(EXAI_INSTALL_TESTS_3RDPARTY)
  exai_install_test(threadpool_tests)
endif()
