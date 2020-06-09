project(epipolarmodel)

add_definitions(-DEXEPIPOLARMODEL_API)
earthxpert_add_library(epipolarmodel "api"
  SRCS "epipolarmodel.cpp"
       "epipolarmodelfactory.cpp"
       "epipolarrect.cpp"
       "frameepipolarmodel.cpp"
       "groundpolicy.cpp"
  ITEMS ex3rdparty core sensormodel 
        ${GDAL_LIBRARY})

earthxpert_install_target(epipolarmodel)
