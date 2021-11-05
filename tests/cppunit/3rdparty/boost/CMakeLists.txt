project(tests_cppunit_3rdparty_boost CXX)

add_3rdparty_cppunit_test(boost_tests
  ex3rdparty
  Boost::boost Boost::date_time Boost::filesystem Boost::iostreams 
  Boost::locale Boost::numpy Boost::program_options Boost::python Boost::timer)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_BOOST_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_BOOST_PATH "${TESTS_BOOST_PATH}")
  message(STATUS "Tests_Boost_Path: ${TESTS_BOOST_PATH}")
  set_tests_properties(boost_tests PROPERTIES ENVIRONMENT "${TESTS_BOOST_PATH}")
endif()

if(EXAI_INSTALL_TESTS_3RDPARTY)
  exai_install_test(boost_tests)
endif()
