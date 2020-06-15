project(epipolar)

add_definitions(-DEXEPIPOLAR_API)
earthxpert_add_library(epipolar "api"
  SRCS "epipolar.cpp"
       "epipolargenerator.cpp"
       "epipolargenerator_layout.cpp"
  ITEMS ex3rdparty dataio sensormodel epipolarmodel marshaller 
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY})

earthxpert_install_target(epipolar)
