project(tests)

set(EXAI_TEST_DATA_DIR "${CMAKE_SOURCE_DIR}/../TestData")
set(EXAI_TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/tests/output")
configure_file("include/testconfig.h.in" ${CMAKE_BINARY_DIR}/tests/include/testconfig.h)
file(MAKE_DIRECTORY ${EXAI_TEST_OUTPUT_DIR})

include_directories(${CMAKE_BINARY_DIR}/tests/include/)
include_directories("include")

function(exai_add_test name tests_dir)
  add_executable(${name} ${name}.cpp)
  set_target_properties(${name}
    PROPERTIES
      CXX_STANDARD 17
      DEBUG_POSTFIX "${EXAI_DEBUG_POSTFIX}")
  target_link_libraries(${name}
    ${GTEST_BOTH_LIBRARIES} ${SLTBENCH_LIBRARIES} ${vld_LIBRARIES}
    ${ARGN})
  add_test(NAME ${name} 
    COMMAND $<TARGET_FILE:${name}>)
  # group tests 
  set_property(TARGET ${name} PROPERTY FOLDER ${tests_dir})
endfunction()

function(exai_install_test name)
  install(TARGETS ${name} ${ARGN}
    RUNTIME DESTINATION ${EXAI_BIN_DIR})
  if(WIN32 AND MSVC AND (NOT MSVC_VERSION LESS 1600))
    install(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${EXAI_BIN_DIR} OPTIONAL)
  endif()
endfunction()

option(EXAI_INSTALL_TESTS_3RDPARTY "install 3rd-party tests" OFF)
option(EXAI_INSTALL_TESTS_CPPUNIT "install C++ unit tests" OFF)
option(EXAI_INSTALL_TESTS_PYUNIT "install python unit tests" OFF)

add_compile_definitions(BOOST_LIB_TOOLSET="vc142")
add_definitions(-D_USE_MATH_DEFINES -DBOOST_ALL_DYN_LINK)
add_definitions(-DMPFR_USE_FILE -DMPFR_USE_INTMAX_T -DMPFR_USE_VA_LIST)

set(TESTS_BIN_DIR ${CMAKE_INSTALL_PREFIX}/${EXAI_BIN_DIR})
set(BOOST_BIN_DIR "${Boost_LIBRARY_DIRS}")
add_subdirectory(src)
#add_subdirectory(acceptance)
add_subdirectory(cppunit)
#add_subdirectory(integration)
add_subdirectory(performance)
#add_subdirectory(pyunit)
#add_subdirectory(regression)

# section install
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
else()
endif()
