project(exgis CXX)

add_definitions(-DEXGIS_API)
set(exgis_srcs 
  "hydro/hydroenforce.cpp")
set(exgui_items ex3rdparty Boost::boost Boost::filesystem Boost::locale 
  glog::glog ${LIBXML2_LIBRARIES} ${OpenCV_LIBS} 
  ${OpenEXR_LIBS})
exai_add_library(exgis "api"
  SRCS ${exgis_srcs}
  ITEMS ${exgis_items})

exai_source_group(${exgis_srcs})
exai_install_target(exgis)
