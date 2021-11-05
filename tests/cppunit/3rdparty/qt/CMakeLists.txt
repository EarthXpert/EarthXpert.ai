project(tests_cppunit_3rdparty_qt CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS})

add_3rdparty_cppunit_test(qt_tests 
  ex3rdparty 
  Qt5::Widgets)

if(WIN32)
  set(TESTS_QT_PATH "PATH=$<TARGET_FILE_DIR:ex3rdparty>;\
${GTEST_BIN_DIR};${TESTS_BIN_DIR};$ENV{PATH}")
  string(REPLACE ";" "\\;" TESTS_QT_PATH "${TESTS_QT_PATH}")
  #message(STATUS "Tests_Qt_Path: ${TESTS_QT_PATH}")
  set_tests_properties(qt_tests PROPERTIES ENVIRONMENT "${TESTS_QT_PATH}")
endif()

if(EXAI_INSTALL_TESTS_3RDPARTY)
  exai_install_test(qt_tests)
endif()
