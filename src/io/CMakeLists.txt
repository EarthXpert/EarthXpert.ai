project(dataio)

add_definitions(-DEXIO_API)
set(dataio_srcs "pointcloud/tiledpointcloudreader.cpp"
  "pointcloud/tiledpointcloudwriter.cpp"
  "pointcloud/pointcloudreader.cpp"
  "pointcloud/pointcloudwriter.cpp"
  "raster/cvrasterreader.cpp"
  "raster/cvrasterwriter.cpp"
  "raster/exairasterreader.cpp"
  "raster/exairasterwriter.cpp"
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
set(dataio_items ex3rdparty core 
  exiv2lib ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} 
  ${LASLIB_LIBRARY} ${OpenCV_LIBS})
earthxpert_add_library(dataio "api"
  SRCS ${dataio_srcs}
  ITEMS ${dataio_items})

earthxpert_source_group(${dataio_srcs})
earthxpert_install_target(dataio)
