project(epipolar CXX)

add_definitions(-DEXEPIPOLAR_API)
set(epipolar_srcs 
  "./epiperf.cpp"
  "./epipolar.cpp"
  "./epipolargenerator.cpp"
  "./epipolargenerator_layout.cpp"
  "./epipolarrectifier.cpp"
  "./epipolartask.cpp"
  "./stereomategenerator.cpp")
set(epipolar_items ex3rdparty core dataio frameworks sensormodel epipolarmodel marshaller 
  ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${OpenCL_LIBS})
exai_add_library(epipolar "api"
  SRCS ${epipolar_srcs}
  ITEMS ${epipolar_items})

exai_source_group(${epipolar_srcs})
exai_install_target(epipolar)
