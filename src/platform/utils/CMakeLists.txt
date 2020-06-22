project(pfutils)

add_definitions(-DEXPFUTILS_API)
earthxpert_add_library(pfutils "platform"
  SRCS "commandinfo.cpp"
       "utils.cpp"
  ITEMS ex3rdparty exutils core dataio epipolar epipolarmodel sensormodel sfm
        Boost::boost Boost::filesystem ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} ${OpenCV_LIBS} 
        ${PDAL_LIBRARIES})

earthxpert_install_target(pfutils)
