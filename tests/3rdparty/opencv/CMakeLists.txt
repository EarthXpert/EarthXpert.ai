project(tests_3rdparty_opencv CXX)

include_directories(${OpenCV_INCLUDE_DIR})

add_3rdparty_test(opencv_tests 
  ex3rdparty 
  ${GDAL_LIBRARY} ${OpenCV_LIBS})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TESTS_OPENCV_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${GDAL_BIN_DIR};${OpenCV_BIN_DIR};${OPENNI_BIN_DIR};${QT_BIN_DIR};\
${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_OPENCV_PATH "${TESTS_OPENCV_PATH}")
  message(STATUS "Tests_OpenCV_Path: ${TESTS_OPENCV_PATH}")
  set_tests_properties(opencv_tests PROPERTIES ENVIRONMENT "${TESTS_OPENCV_PATH}")
endif()

if(EARTHXPERT_INSTALL_TESTS_3RDPARTY)
  earthxpert_install_test(opencv_tests)
endif()
