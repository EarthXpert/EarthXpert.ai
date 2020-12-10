project(ortho CXX)

add_definitions(-DEXORTHO_API)
set(ortho_srcs 
  "./orthorectifier.cpp"
  "./orthotask.cpp")
earthxpert_add_library(ortho "api"
  SRCS ${ortho_srcs}
  ITEMS ex3rdparty core dataio frameworks sensormodel marshaller 
        ${GDAL_LIBRARY} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} ${OpenCL_LIBS})

earthxpert_source_group(${ortho_srcs})
earthxpert_install_target(ortho)
