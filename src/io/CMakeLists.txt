project(dataio)

add_definitions(-DEXIO_API)
earthxpert_add_library(dataio "api"
  SRCS "pointcloud/pointcloudreader.cpp"
       "pointcloud/pointcloudwriter.cpp"
       "raster/rasterreader.cpp"
       "raster/rasterwriter.cpp"
  ITEMS ex3rd_party core 
        ${GDAL_LIBRARY} ${LASLIB_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(dataio)
