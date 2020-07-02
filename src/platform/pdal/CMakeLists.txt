project(pfpdal)

add_definitions(-DEXPDAL_API)
earthxpert_add_library(pfpdal "platform"
  SRCS "pdaljob.cpp"
       "pdalmanager.cpp"
  ITEMS ex3rdparty core dataio pfutils
        Boost::boost Boost::filesystem Boost::program_options ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} ${PDAL_LIBRARIES})

earthxpert_install_target(pfpdal)
