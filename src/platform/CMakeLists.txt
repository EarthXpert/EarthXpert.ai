project(platform)

add_definitions(-DEXPLATFORM_API)
earthxpert_add_library(platform "api"
  SRCS "managers/exaimanager.cpp"
       "managers/gdalmanager.cpp"
       "services/exaijob.cpp"
       "services/gdaljob.cpp"
       "services/pdaljob.cpp"
       "services/servicepipeline.cpp"
  ITEMS ex3rdparty exutils core dataio epipolar epipolarmodel sensormodel sfm
        Boost::boost Boost::filesystem ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY} ${OpenCV_LIBS} 
        ${PDAL_LIBRARIES})

earthxpert_install_target(platform)
