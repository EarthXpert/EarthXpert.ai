project(pfpdal)

add_definitions(-DEXPDAL_API)
set(pfpdal_srcs "pdaljob.cpp"
  "pdalmanager.cpp")
earthxpert_add_library(pfpdal "platform"
  SRCS ${pfpdal_srcs}
  ITEMS ex3rdparty core dataio pfutils
        Boost::boost Boost::filesystem Boost::program_options ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} ${PDAL_LIBRARIES})

earthxpert_source_group(${pfpdal_srcs})
earthxpert_install_target(pfpdal)
