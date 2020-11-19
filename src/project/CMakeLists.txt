project(exproject)

add_definitions(-DEXPROJECT_API)
set(exproject_srcs "./project.cpp"
  "ads/adsprj.cpp"
  "frame/frameprj.cpp"
  "rational/rationalprj.cpp")
earthxpert_add_library(exproject "api"
  SRCS ${exproject_srcs}
  ITEMS ex3rdparty core sensormodel
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${geotrans_LIBRARIES} 
        ${GETTEXT_LIBRARY} ${OpenCV_LIBS})

earthxpert_source_group(${exproject_srcs})
earthxpert_install_target(exproject)
