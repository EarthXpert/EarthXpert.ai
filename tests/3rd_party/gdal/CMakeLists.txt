project(tests_3rd_party_gdal CXX)

include_directories(${GDAL_INCLUDE_DIR})

add_3rd_party_test(gdal_tests 
  ${GDAL_LIBRARY})

if(WIN32)
  set(TESTS_GDAL_PATH "PATH=${GTEST_BIN_DIR};${GDAL_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_GDAL_PATH "${TESTS_GDAL_PATH}")
  #message(STATUS "Tests_Gdal_Path: ${TESTS_GDAL_PATH}")
  set_tests_properties(gdal_tests PROPERTIES ENVIRONMENT "${TESTS_GDAL_PATH}")
endif()
