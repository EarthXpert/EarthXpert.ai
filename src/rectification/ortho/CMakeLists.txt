project(ortho CXX)

add_definitions(-DEXORTHO_API)
set(ortho_srcs 
  "./orthorectifier.cpp"
  "./orthotask.cpp")
set(ortho_items ex3rdparty core dataio exdem frameworks sensormodel marshaller 
  ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} ${OpenCL_LIBS})
earthxpert_add_library(ortho "api"
  SRCS ${ortho_srcs}
  ITEMS ${ortho_items})

earthxpert_source_group(${ortho_srcs})
earthxpert_install_target(ortho)
