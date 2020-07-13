project(gui CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS})

earthxpert_add_executable(exqt5_hw "apps/gui"
  SRCS "qt5_hw.cpp"
       "mainwindow.cpp"
  ITEMS ${GETTEXT_LIBRARY} Qt5::Widgets)

earthxpert_install_target(exqt5_hw)
