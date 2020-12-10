project(tortho CXX)

add_definitions(-DEXTORTHO_API)
set(tortho_srcs 
  "./torthorectifier.cpp"
  "./torthotask.cpp")
earthxpert_add_library(tortho "api"
  SRCS ${tortho_srcs}
  ITEMS ex3rdparty core dataio ortho frameworks sensormodel 
        ${GDAL_LIBRARY} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} ${OpenCL_LIBS})

earthxpert_source_group(${tortho_srcs})
earthxpert_install_target(tortho)
