project(blockadjust)

add_definitions(-DEXBBA_API)
set(blockadjust_srcs "blockadjust.cpp"
       "ads/adsbba.cpp"
       "frame/framebba.cpp"
       "frame/adjustframe.cpp"
       "rational/rationalbba.cpp")
earthxpert_add_library(blockadjust "api"
  SRCS ${blockadjust_srcs}
  ITEMS ex3rdparty core exproject sensormodel
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCV_LIBS})

earthxpert_source_group(${blockadjust_srcs})
earthxpert_install_target(blockadjust)
