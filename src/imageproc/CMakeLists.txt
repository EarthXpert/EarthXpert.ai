project(imageproc CXX)

add_definitions(-DEXIMAGEPROC_API)
set(imageproc_srcs 
  "./opencvutils.cpp"
  "algorithm/clouddetect.cpp"
  "algorithm/dehaze.cpp"
  "algorithm/wallisfilter.cpp"
  "algorithm/waterbodydetect.cpp"
  "algorithm/superres.cpp"
  "feature/buildingdetect.cpp"
  "feature/featdetectperf.cpp"
  "feature/featuredetect.cpp"
  "feature/featuredetect_layout.cpp"
  "feature/roaddetect.cpp"
  "filter/atcomp.cpp"
  "filter/imagefilter.cpp"
  "utils/utils.cpp")
set(imageproc_items ex3rdparty core dataio 
        ${GDAL_LIBRARIES} ${OpenCL_LIBS} ${OpenCV_LIBS})
exai_add_library(imageproc "api"
  SRCS ${imageproc_srcs}
  ITEMS ${imageproc_items})

exai_source_group(${imageproc_srcs})
exai_install_target(imageproc)
