project(pfutils)

add_definitions(-DEXPFUTILS_API)
set(pfutils_srcs "./commandinfo.cpp"
  "./pfutils.cpp")
set(_items ex3rdparty core dataio epipolar epipolarmodel exdem exutils sensormodel sfm
  Boost::boost Boost::filesystem ${CERES_LIBRARY} ${CGAL_LIBRARIES} 
  ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} glog::glog 
  ${LASLIB_LIBRARY} ${OpenCV_LIBS} ${PDAL_LIBRARIES} ${zlib_LIBRARIES})
earthxpert_add_library(pfutils "platform"
  SRCS ${pfutils_srcs}
  ITEMS ${pfutils_items})

earthxpert_source_group(${pfutils_srcs})
earthxpert_install_target(pfutils)
