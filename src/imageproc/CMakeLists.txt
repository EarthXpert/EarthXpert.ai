project(imageproc CXX)

add_definitions(-DEXIMAGEPROC_API)
set(imageproc_srcs 
  "./opencvutils.cpp"
  "algorithm/clouddetect.cpp"
  "algorithm/dehaze.cpp"
  "algorithm/wallisfilter.cpp"
  "algorithm/waterbodydetect.cpp"
  "algorithm/superres.cpp"
  "filter/atcomp.cpp"
  "filter/imagefilter.cpp")
set(imageproc_items ex3rdparty core  
        ${GDAL_LIBRARIES} ${OpenCL_LIBS} ${OpenCV_LIBS})
exai_add_library(imageproc "api"
  SRCS ${imageproc_srcs}
  ITEMS ${imageproc_items})

exai_source_group(${imageproc_srcs})
exai_install_target(imageproc)
