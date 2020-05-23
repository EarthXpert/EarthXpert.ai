project(gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS})

watxtract_add_executable(exgui "apps"
  SRCS "gui.cpp"
  ITEMS core)
watxtract_add_executable(exqt5_hw "apps"
  SRCS "qt5_hw.cpp"
  ITEMS Qt5::Widgets)

watxtract_install_target(exgui)
watxtract_install_target(exqt5_hw)
