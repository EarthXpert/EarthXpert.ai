project(dataio)

add_definitions(-DWXIO_API)
watxtract_add_library(dataio "api"
  SRCS "pc/pointcloudreader.cpp"
       "pc/pointcloudwriter.cpp"
       "pointcloud/pointcloudreader.cpp"
       "pointcloud/pointcloudwriter.cpp"
       "raster/imagereader.cpp"
       "raster/imagewriter.cpp"
       "raster/rasterreader.cpp"
       "raster/rasterwriter.cpp"
  ITEMS wx3rd_party core 
        ${GDAL_LIBRARY} ${LASLIB_LIBRARY} ${OPENCV_LIBS})

watxtract_install_target(dataio)
