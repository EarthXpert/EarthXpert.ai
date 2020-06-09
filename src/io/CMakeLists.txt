project(dataio)

add_definitions(-DEXIO_API)
earthxpert_add_library(dataio "api"
  SRCS "pointcloud/pointcloudreader.cpp"
       "pointcloud/pointcloudwriter.cpp"
       "raster/rasterreader.cpp"
       "raster/rasterwriter.cpp"
       "vector/vectorreader.cpp"
       "vector/vectorwriter.cpp"
  ITEMS ex3rdparty core 
        ${GDAL_LIBRARY} ${LASLIB_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(dataio)
