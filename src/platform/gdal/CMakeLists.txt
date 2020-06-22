project(pfgdal)

add_definitions(-DEXGDAL_API)
earthxpert_add_library(pfgdal "platform"
  SRCS "gdaljob.cpp"
       "gdalmanager.cpp"
  ITEMS ex3rdparty core pfutils 
        Boost::boost Boost::filesystem ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog)

earthxpert_install_target(pfgdal)
