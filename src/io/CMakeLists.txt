project(dataio)

add_definitions(-DEXIO_API)
set(dataio_srcs "pointcloud/tiledpointcloudreader.cpp"
       "pointcloud/tiledpointcloudwriter.cpp"
       "pointcloud/pointcloudreader.cpp"
       "pointcloud/pointcloudwriter.cpp"
       "raster/tiledrasterreader.cpp"
       "raster/tiledrasterwriter.cpp"
       "raster/rasterreader.cpp"
       "raster/rasterwriter.cpp"
       "terrain/demreader.cpp"
       "terrain/demwriter.cpp"
       "terrain/tileddemreader.cpp"
       "terrain/tileddemwriter.cpp"
       "vector/tiledvectorreader.cpp"
       "vector/tiledvectorwriter.cpp"
       "vector/vectorreader.cpp"
       "vector/vectorwriter.cpp")
earthxpert_add_library(dataio "api"
  SRCS ${dataio_srcs}
  ITEMS ex3rdparty core 
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${LASLIB_LIBRARY} ${OpenCV_LIBS})

earthxpert_source_group(${dataio_srcs})
earthxpert_install_target(dataio)
