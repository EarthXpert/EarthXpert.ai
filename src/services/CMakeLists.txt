project(pfservices CXX)

add_definitions(-DEXSERVICES_API)
set(pfservices_srcs "./service.cpp"
  "./webservice.cpp")
set(pfservices_items ex3rdparty exutils core dataio epipolar epipolarmodel 
  pfutils sensormodel sfm
  Boost::boost Boost::filesystem ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARIES} 
  ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} 
  ${OpenCV_LIBS} ${PDAL_LIBRARIES})
exai_add_library(pfservices "platform"
  SRCS ${pfservices_srcs}
  ITEMS ${pfservices_items})

exai_install_target(pfservices)
