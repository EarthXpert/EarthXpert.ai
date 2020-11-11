project(pfutils)

add_definitions(-DEXPFUTILS_API)
set(pfutils_srcs "./commandinfo.cpp"
  "./pfutils.cpp")
earthxpert_add_library(pfutils "platform"
  SRCS ${pfutils_srcs}
  ITEMS ex3rdparty exutils core dataio epipolar epipolarmodel sensormodel sfm
        Boost::boost Boost::filesystem ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} ${OpenCV_LIBS} 
        ${PDAL_LIBRARIES})

earthxpert_source_group(${pfutils_srcs})
earthxpert_install_target(pfutils)
