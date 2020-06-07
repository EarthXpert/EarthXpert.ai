project(blockadjust)

add_definitions(-DEXBBA_API)
earthxpert_add_library(blockadjust "api"
  SRCS "blockadjust.cpp"
       "ads/adsbba.cpp"
       "frame/framebba.cpp"
       "rational/rationalbba.cpp"
  ITEMS ex3rd_party core sensormodel
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(blockadjust)
