project(epipolarmodel)

add_definitions(-DEXEPIPOLARMODEL_API)
earthxpert_add_library(epipolarmodel "api"
  SRCS "epipolarmodel.cpp"
       "epipolarmodelfactory.cpp"
       "epipolarrect.cpp"
       "epipolarsensormodel.cpp"
       "frameepipolarmodel.cpp"
       "utils.cpp"
  ITEMS ex3rdparty core sensormodel 
        Boost::boost ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCV_LIBS})

earthxpert_install_target(epipolarmodel)
