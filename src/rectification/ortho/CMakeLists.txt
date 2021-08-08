project(ortho CXX)

add_definitions(-DEXORTHO_API)
set(ortho_srcs 
  "./ortho.cpp"
  "./orthorect.cpp"
  "./orthorectifier.cpp"
  "./orthotask.cpp")
set(ortho_items ex3rdparty core dataio exdem frameworks sensormodel marshaller 
  ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${OpenCL_LIBS})
exai_add_library(ortho "api"
  SRCS ${ortho_srcs}
  ITEMS ${ortho_items})

exai_source_group(${ortho_srcs})
exai_install_target(ortho)
