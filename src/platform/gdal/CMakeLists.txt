project(pfgdal)

add_definitions(-DEXGDAL_API)
earthxpert_add_library(pfgdal "platform"
  SRCS "gdalinfo.cpp"
       "gdaljob.cpp"
       "gdalmanager.cpp"
       "gdalsrsinfo.cpp"
       "gdaltranslate.cpp"
       "ogrinfo.cpp"
  ITEMS ex3rdparty core pfutils 
        Boost::boost Boost::filesystem ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog)

earthxpert_install_target(pfgdal)
