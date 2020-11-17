project(epipolarmodel)

add_definitions(-DEXEPIPOLARMODEL_API)
set(epipolarmodel_srcs "./epipolarmodel.cpp"
  "./epipolarrect.cpp"
  "./epipolarstereofactory.cpp"
  "./frameepipolarmodel.cpp"
  "./sensorepipolarmodel.cpp"
  "./stereomatemodel.cpp"
  "utils/epiutils.cpp"
  "utils/evalepipolarmodel.cpp")
earthxpert_add_library(epipolarmodel "api"
  SRCS ${epipolarmodel_srcs}
  ITEMS ex3rdparty core sensormodel 
        Boost::boost ${GDAL_LIBRARY} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} 
        ${OpenCV_LIBS})

earthxpert_source_group(${epipolarmodel_srcs})
earthxpert_install_target(epipolarmodel)
