project(epipolarmodel)

add_definitions(-DEXEPIPOLARMODEL_API)
set(epipolarmodel_srcs "epipolarmodel.cpp"
       "epipolarmodelfactory.cpp"
       "epipolarrect.cpp"
       "epipolarsensormodel.cpp"
       "frameepipolarmodel.cpp"
       "utils.cpp")
earthxpert_add_library(epipolarmodel "api"
  SRCS ${epipolarmodel_srcs}
  ITEMS ex3rdparty core sensormodel 
        Boost::boost ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCV_LIBS})

earthxpert_source_group(${epipolarmodel_srcs})
earthxpert_install_target(epipolarmodel)
