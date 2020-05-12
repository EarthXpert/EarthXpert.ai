project(dataio)

add_definitions(-DWXIO_API)
watxtract_add_library(dataio "api"
  SRCS "pointcloud/pointcloudreader.cpp"
       "pointcloud/pointcloudwriter.cpp"
       "raster/rasterreader.cpp"
       "raster/rasterwriter.cpp"
  ITEMS wx3rd_party core 
        ${GDAL_LIBRARY} ${LASLIB_LIBRARY} ${OPENCV_LIBS})

watxtract_install_target(dataio)
