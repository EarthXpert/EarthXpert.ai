project(imageproc CXX)

add_definitions(-DEXIMAGEPROC_API)
set(imageproc_srcs "algorithm/clouddetect.cpp"
  "algorithm/wallisfilter.cpp"
  "algorithm/waterbodydetect.cpp"
  "filter/dehaze.cpp"
  "filter/imagefilter.cpp")
earthxpert_add_library(imageproc "api"
  SRCS ${imageproc_srcs}
  ITEMS ex3rdparty core  
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCL_LIBS} ${OpenCV_LIBS})

earthxpert_source_group(${imageproc_srcs})
earthxpert_install_target(imageproc)
