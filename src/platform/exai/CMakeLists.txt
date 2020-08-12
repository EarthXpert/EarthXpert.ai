project(pfexai)

add_definitions(-DEXEXAI_API)
set(pfexai_srcs "exaiblockadjust.cpp"
       "exaicmd.cpp"
       "exaiepipolar.cpp"
       "exaijob.cpp"
       "exaimanager.cpp")
earthxpert_add_library(pfexai "platform"
  SRCS ${pfexai_srcs}
  ITEMS ex3rdparty blockadjust exutils core dataio epipolar epipolarmodel exproject pfutils sensormodel sfm
        Boost::boost Boost::filesystem Boost::program_options ${CERES_LIBRARY} 
        ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} glog::glog 
        ${LASLIB_LIBRARY} ${OpenCV_LIBS} ${PDAL_LIBRARIES})

earthxpert_source_group(${pfexai_srcs})
earthxpert_install_target(pfexai)
