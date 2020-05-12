project(epipolarmodel)

add_definitions(-DWXEPIPOLARMODEL_API)
watxtract_add_library(epipolarmodel "api"
  SRCS "epipolarmodel.cpp"
       "epipolarmodelfactory.cpp"
       "epipolarrect.cpp"
       "frameepipolarmodel.cpp"
       "groundpolicy.cpp"
  ITEMS wx3rd_party core sensormodel 
        ${GDAL_LIBRARY})

watxtract_install_target(epipolarmodel)
