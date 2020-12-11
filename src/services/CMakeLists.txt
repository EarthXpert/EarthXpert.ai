project(pfservices)

add_definitions(-DEXSERVICES_API)
set(pfservices_items ex3rdparty exutils core dataio epipolar epipolarmodel 
  pfutils sensormodel sfm
  Boost::boost Boost::filesystem ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} 
  ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} 
  ${OpenCV_LIBS} ${PDAL_LIBRARIES})
earthxpert_add_library(pfservices "platform"
  SRCS "./service.cpp"
  ITEMS ${pfservices_items})

earthxpert_install_target(pfservices)
