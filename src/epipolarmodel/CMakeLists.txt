project(epipolarmodel CXX)

add_definitions(-DEXEPIPOLARMODEL_API)
set(epipolarmodel_srcs 
  "./epipolarmodel.cpp"
  "./epipolarrect.cpp"
  "./epipolarstereofactory.cpp"
  "./frameepipolarmodel.cpp"
  "./sensorepipolarmodel.cpp"
  "./stereomatemodel.cpp"
  "utils/epiutils.cpp"
  "utils/evalepipolarmodel.cpp")
exai_add_library(epipolarmodel "api"
  SRCS ${epipolarmodel_srcs}
  ITEMS ex3rdparty core sensormodel 
        Boost::boost ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} glog::glog
        ${OpenCV_LIBS})

exai_source_group(${epipolarmodel_srcs})
exai_install_target(epipolarmodel)
