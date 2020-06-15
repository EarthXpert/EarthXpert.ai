project(exutils)

add_definitions(-DEXUTILS_API)
earthxpert_add_library(exutils "api"
  SRCS "pointcloud/pointcloudrasterize.cpp"
  ITEMS ex3rdparty core dataio
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${LASLIB_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(exutils)
