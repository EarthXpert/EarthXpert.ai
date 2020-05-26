project(exutils)

add_definitions(-DEXUTILS_API)
earthxpert_add_library(exutils "api"
  SRCS "pointcloud/pointcloudrasterize.cpp"
  ITEMS ex3rd_party core dataio
        ${GDAL_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(exutils)
