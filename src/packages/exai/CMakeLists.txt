project(pfexai CXX)

add_definitions(-DEXEXAI_API)
set(pfexai_srcs "exaiblockadjust.cpp"
  "exaicmd.cpp"
  "exaicolorbal.cpp"
  "exaiepipolar.cpp"
  "exaijob.cpp"
  "exaimanager.cpp")
set(pfexai_items ex3rdparty blockadjust exutils colorbal core dataio epipolar 
  epipolarmodel exproject imageproc pcproc pfutils sensormodel sfm
  Boost::boost Boost::filesystem Boost::program_options ${CERES_LIBRARY} 
  ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} 
  glog::glog ${ICONV_LIBRARIES} ${LASLIB_LIBRARY} ${LIBXML2_LIBRARIES}
  ${OpenCV_LIBS} ${PDAL_LIBRARIES})
earthxpert_add_library(pfexai "platform"
  SRCS ${pfexai_srcs}
  ITEMS ${pfexai_items})

earthxpert_source_group(${pfexai_srcs})
earthxpert_install_target(pfexai)
