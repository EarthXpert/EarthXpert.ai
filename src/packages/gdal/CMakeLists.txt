project(pfgdal)

add_definitions(-DEXGDAL_API)
set(pfgdal_srcs "gdalinfo.cpp"
  "gdaljob.cpp"
  "gdalmanager.cpp"
  "gdalsrsinfo.cpp"
  "gdaltranslate.cpp"
  "ogrinfo.cpp")
set(pfgdal_items ex3rdparty core pfutils 
  Boost::boost Boost::filesystem Boost::program_options ${GDAL_LIBRARY} 
  ${GETTEXT_LIBRARY} glog::glog)
earthxpert_add_library(pfgdal "platform"
  SRCS ${pfgdal_srcs}
  ITEMS ${pfgdal_items})

earthxpert_source_group(${pfgdal_srcs})
earthxpert_install_target(pfgdal)
