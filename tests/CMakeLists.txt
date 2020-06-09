project(tests)

set(EARTHXPERT_TEST_DATA_DIR "${CMAKE_SOURCE_DIR}/../TestData")
set(EARTHXPERT_TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/tests/output")
configure_file(testconfig.h.in ${CMAKE_BINARY_DIR}/tests/testconfig.h)
file(MAKE_DIRECTORY ${EARTHXPERT_TEST_OUTPUT_DIR})

include_directories(${CMAKE_BINARY_DIR}/tests/)

function(earthxpert_add_test name tests_dir)
  add_executable(${name} ${name}.cpp)
  set_target_properties(${name}
    PROPERTIES
      CXX_STANDARD 17
      DEBUG_POSTFIX "${EARTHXPERT_DEBUG_POSTFIX}")
  target_link_libraries(${name}
    ${GTEST_BOTH_LIBRARIES} ${SLTBENCH_LIBS}
    ${ARGN})
  add_test(NAME ${name} 
    COMMAND $<TARGET_FILE:${name}>)
  # group tests 
  set_property(TARGET ${name} PROPERTY FOLDER ${tests_dir})
endfunction()

function(earthxpert_install_test name)
  install(TARGETS ${name} ${ARGN}
    RUNTIME DESTINATION ${EARTHXPERT_BIN_DIR})
  if(WIN32 AND MSVC AND (NOT MSVC_VERSION LESS 1600))
    install(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${EARTHXPERT_BIN_DIR} OPTIONAL)
  endif()
endfunction()

option(EARTHXPERT_INSTALL_TESTS_3RDPARTY "install 3rd-party tests" OFF)
option(EARTHXPERT_INSTALL_TESTS_CPPUNIT "install C++ unit tests" OFF)
option(EARTHXPERT_INSTALL_TESTS_PYUNIT "install python unit tests" OFF)

set(TESTS_BIN_DIR ${CMAKE_INSTALL_PREFIX}/${EARTHXPERT_BIN_DIR})
set(BOOST_BIN_DIR "${Boost_LIBRARY_DIRS}")
add_subdirectory(3rdparty)
add_subdirectory(cppunit)
add_subdirectory(performance)
#add_subdirectory(pyunit)

# section install
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
else()
endif()
