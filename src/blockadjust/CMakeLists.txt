project(blockadjust CXX)

add_definitions(-DEXBBA_API)
set(blockadjust_srcs "./blockadjust.cpp"
  "ads/adjustads.cpp"
  "ads/adsbba.cpp"
  "frame/adjustframe.cpp"
  "frame/framebba.cpp"
  "rational/adjustrational.cpp"
  "rational/rationalbba.cpp")
earthxpert_add_library(blockadjust "api"
  SRCS ${blockadjust_srcs}
  ITEMS ex3rdparty core exproject sensormodel
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${geotrans_LIBRARIES} 
        ${GETTEXT_LIBRARY} ${MPFR4_LIBRARIES} ${MPIR_LIBRARIES} ${OpenCV_LIBS})

earthxpert_source_group(${blockadjust_srcs})
earthxpert_install_target(blockadjust)
