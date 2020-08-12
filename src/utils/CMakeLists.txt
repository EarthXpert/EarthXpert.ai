project(exutils)

add_definitions(-DEXUTILS_API)
set(exutils_srcs "pointcloud/pointcloudrasterize.cpp")
earthxpert_add_library(exutils "api"
  SRCS ${exutils_srcs}
  ITEMS ex3rdparty core dataio
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${LASLIB_LIBRARY} ${OpenCV_LIBS})

earthxpert_source_group(${exutils_srcs})
earthxpert_install_target(exutils)
