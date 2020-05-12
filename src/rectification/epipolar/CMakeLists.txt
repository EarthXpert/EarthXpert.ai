project(epipolar)

add_definitions(-DWXEPIPOLAR_API)
watxtract_add_library(epipolar "api"
  SRCS "epipolar.cpp"
       "epipolargenerator.cpp"
       "epipolargenerator_layout.cpp"
  ITEMS wx3rd_party dataio sensormodel epipolarmodel marshaller 
        ${GDAL_LIBRARY})

watxtract_install_target(epipolar)
