project(epipolar CXX)

add_definitions(-DEXEPIPOLAR_API)
set(epipolar_srcs "epiperf.cpp"
       "epipolar.cpp"
       "epipolargenerator.cpp"
       "epipolargenerator_layout.cpp")
earthxpert_add_library(epipolar "api"
  SRCS ${epipolar_srcs}
  ITEMS ex3rdparty dataio sensormodel epipolarmodel marshaller 
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCL_LIBS})

earthxpert_source_group(${epipolar_srcs})
earthxpert_install_target(epipolar)
