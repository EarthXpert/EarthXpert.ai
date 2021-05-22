project(gui CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS})

add_gui_executable(exqt5_hw 
  SRCS "qt5_hw.cpp"
       "mainwindow.cpp"
  ITEMS ex3rdparty exgui Qt5::Widgets)

exai_install_target(exqt5_hw)
