project(pfpdal CXX)

add_definitions(-DEXPDAL_API)
set(pfpdal_srcs 
  "pdaljob.cpp"
  "pdalmanager.cpp")
set(pfpdal_items ex3rdparty core dataio pfutils
  Boost::boost Boost::filesystem Boost::program_options ${GDAL_LIBRARIES} 
  ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} ${PDAL_LIBRARIES})
exai_add_library(pfpdal "platform"
  SRCS ${pfpdal_srcs}
  ITEMS ${pfpdal_items})

exai_source_group(${pfpdal_srcs})
exai_install_target(pfpdal)
