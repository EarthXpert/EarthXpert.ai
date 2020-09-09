project(exgui CXX)

add_definitions(-DEXGUI_API)
set(exgui_srcs "qt/singleinstance.cpp")
earthxpert_add_library(exgui "api"
  SRCS ${exgui_srcs}
  ITEMS ex3rdparty Boost::boost Boost::filesystem Boost::locale ${GETTEXT_LIBRARY} 
        glog::glog ${LIBXML2_LIBRARIES} ${OpenCV_LIBS} ${OpenEXR_LIBS} 
        Qt5::Widgets)

earthxpert_source_group(${exgui_srcs})
earthxpert_install_target(exgui)
