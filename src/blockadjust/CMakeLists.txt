project(blockadjust CXX)

add_definitions(-DEXBBA_API)
set(blockadjust_srcs "./blockadjust.cpp"
  "./ceressolver.cpp"
  "./eigensolver.cpp"
  "ads/adjustads.cpp"
  "ads/adsbba.cpp"
  "frame/adjustframe.cpp"
  "frame/framebba.cpp"
  "frame/framebbaimage.cpp"
  "frame/framebbaimageset.cpp"
  "frame/framebbaparam.cpp"
  "insar/adjustinsar.cpp"
  "insar/insarbba.cpp"
  "insar/insarbbaimage.cpp"
  "insar/insarbbaimageset.cpp"
  "insar/insarbbaparam.cpp"
  "rational/adjustrational.cpp"
  "rational/rationalbba.cpp"
  "rational/rationalbbaimage.cpp"
  "rational/rationalbbaimageset.cpp"
  "rational/rationalbbaparam.cpp"
  "rational/rationalceressolver.cpp"
  "sar/adjustsar.cpp"
  "sar/sarbba.cpp"
  "sar/sarbbaimage.cpp"
  "sar/sarbbaimageset.cpp"
  "sar/sarbbaparam.cpp")
set(blockadjust_items ex3rdparty core exproject sensormodel
  Boost::boost Boost::filesystem Boost::iostreams
  ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} 
  ${GETTEXT_LIBRARY} ${ICONV_LIBRARIES} ${LIBXML2_LIBRARIES} 
  ${MPFR4_LIBRARIES} ${MPIR_LIBRARIES} ${OpenCV_LIBS})
earthxpert_add_library(blockadjust "api"
  SRCS ${blockadjust_srcs}
  ITEMS ${blockadjust_items})

earthxpert_source_group(${blockadjust_srcs})
earthxpert_install_target(blockadjust)
