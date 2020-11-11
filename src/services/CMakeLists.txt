project(pfservices)

add_definitions(-DEXSERVICES_API)
earthxpert_add_library(pfservices "platform"
  SRCS "./pipeline.cpp"
  ITEMS ex3rdparty exutils core dataio epipolar epipolarmodel pfutils sensormodel sfm
        Boost::boost Boost::filesystem ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} ${OpenCV_LIBS} 
        ${PDAL_LIBRARIES})

earthxpert_install_target(pfservices)
