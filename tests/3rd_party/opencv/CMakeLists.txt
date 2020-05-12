project(tests_3rd_party_opencv CXX)

include_directories(${OPENCV_INCLUDE_DIR})

add_3rd_party_test(opencv_tests 
  wx3rd_party 
  ${GDAL_LIBRARY} ${OPENCV_LIBS})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TESTS_OPENCV_PATH "PATH=$<TARGET_FILE_DIR:wx3rd_party>;\
${GTEST_BIN_DIR};${GDAL_BIN_DIR};${OPENCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_OPENCV_PATH "${TESTS_OPENCV_PATH}")
  message(STATUS "Tests_OpenCV_Path: ${TESTS_OPENCV_PATH}")
  set_tests_properties(opencv_tests PROPERTIES ENVIRONMENT "${TESTS_OPENCV_PATH}")
endif()

if(WATXTRACT_INSTALL_TESTS_3RDPARTY)
  watxtract_install_test(opencv_tests)
endif()
