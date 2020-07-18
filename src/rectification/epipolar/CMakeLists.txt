project(epipolar)

add_definitions(-DEXEPIPOLAR_API)
earthxpert_add_library(epipolar "api"
  SRCS "epipolar.cpp"
       "epipolargenerator.cpp"
       "utils.cpp"
  ITEMS ex3rdparty dataio sensormodel epipolarmodel marshaller 
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCL_LIBS})

earthxpert_install_target(epipolar)
