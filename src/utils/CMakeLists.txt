project(exutils CXX)

add_definitions(-DEXUTILS_API)
set(exutils_srcs "pointcloud/pointcloudrasterize.cpp")
set(exutils_items ex3rdparty core dataio exdem sensormodel
  Boost::boost Boost::filesystem Boost::iostreams
  ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES}
  ${GETTEXT_LIBRARY} ${LASLIB_LIBRARY} ${OpenCV_LIBS})
exai_add_library(exutils "api"
  SRCS ${exutils_srcs}
  ITEMS ${exutils_items})

exai_source_group(${exutils_srcs})
exai_install_target(exutils)
