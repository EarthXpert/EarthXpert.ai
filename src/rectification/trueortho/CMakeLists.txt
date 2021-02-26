project(tortho CXX)

add_definitions(-DEXTORTHO_API)
set(tortho_srcs 
  "./torthorectifier.cpp"
  "./torthotask.cpp")
set(tortho_items ex3rdparty core dataio ortho frameworks sensormodel 
  ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} ${OpenCL_LIBS})
earthxpert_add_library(tortho "api"
  SRCS ${tortho_srcs}
  ITEMS ${tortho_items})

earthxpert_source_group(${tortho_srcs})
earthxpert_install_target(tortho)
