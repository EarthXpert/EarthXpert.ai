project(exgui CXX)

add_definitions(-DEXGUI_API)
set(exgui_srcs 
  "qt/singleinstance.cpp")
set(exgui_items ex3rdparty Boost::boost Boost::filesystem Boost::locale 
  glog::glog ${LIBXML2_LIBRARIES} ${OpenCV_LIBS} 
  ${OpenEXR_LIBS} Qt5::Widgets)
exai_add_library(exgui "api"
  SRCS ${exgui_srcs}
  ITEMS ${exgui_items})

exai_source_group(${exgui_srcs})
exai_install_target(exgui)
