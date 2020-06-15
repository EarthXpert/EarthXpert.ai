project(exproject)

add_definitions(-DEXPROJECT_API)
earthxpert_add_library(exproject "api"
  SRCS "project.cpp"
       "ads/adsprj.cpp"
       "frame/frameprj.cpp"
       "rational/rationalprj.cpp"
  ITEMS ex3rdparty core sensormodel
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(exproject)
