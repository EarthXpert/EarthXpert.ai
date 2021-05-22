project(exproject CXX)

add_definitions(-DEXPROJECT_API)
set(exproject_srcs 
  "./prjimage.cpp"
  "./project.cpp"
  "ads/adsprj.cpp"
  "frame/frameprj.cpp"
  "rational/rationalprj.cpp"
  "rational/rationalprjimage.cpp"
  "sar/sarprj.cpp"
  "sar/sarprjimage.cpp")
set(exproject_items ex3rdparty core sensormodel
  Boost::boost Boost::filesystem Boost::iostreams
  ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} 
  ${ICONV_LIBRARIES} ${LIBXML2_LIBRARIES} ${OpenCV_LIBS})
exai_add_library(exproject "api"
  SRCS ${exproject_srcs}
  ITEMS ${exproject_items})

exai_source_group(${exproject_srcs})
exai_install_target(exproject)
