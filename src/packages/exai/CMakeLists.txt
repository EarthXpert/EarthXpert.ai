project(pfexai CXX)

add_definitions(-DEXEXAI_API)
set(pfexai_srcs 
  "exaiblockadjust.cpp"
  "exaicmd.cpp"
  "exaicolorbal.cpp"
  "exaiepipolar.cpp"
  "exaijob.cpp"
  "exaimanager.cpp"
  "exaiortho.cpp")
set(pfexai_items ex3rdparty blockadjust colorbal core dataio epipolar 
  epipolarmodel exdem exproject exutils imageproc ortho pcproc pfutils sensormodel sfm
  Boost::boost Boost::filesystem Boost::program_options ${CERES_LIBRARY} 
  ${CGAL_LIBRARIES} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} 
  glog::glog ${ICONV_LIBRARIES} ${LASLIB_LIBRARY} ${LIBXML2_LIBRARIES}
  ${OpenCV_LIBS} ${PDAL_LIBRARIES})
exai_add_library(pfexai "platform"
  SRCS ${pfexai_srcs}
  ITEMS ${pfexai_items})

exai_source_group(${pfexai_srcs})
exai_install_target(pfexai)
