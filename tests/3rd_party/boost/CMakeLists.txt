project(tests_3rd_party_boost CXX)

add_compile_definitions(BOOST_LIB_TOOLSET="vc142")
add_definitions(-DBOOST_ALL_DYN_LINK)
add_3rd_party_test(boost_tests
  Boost::filesystem Boost::iostreams)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  get_filename_component(BOOST_BIN_DIR "${Boost_DIR}/../.." ABSOLUTE)
  set(TESTS_BOOST_PATH "PATH=${GTEST_BIN_DIR};${BOOST_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_BOOST_PATH "${TESTS_BOOST_PATH}")
  message(STATUS "Tests_Boost_Path: ${TESTS_BOOST_PATH}")
  set_tests_properties(boost_tests PROPERTIES ENVIRONMENT "${TESTS_BOOST_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(boost_tests)
endif()
