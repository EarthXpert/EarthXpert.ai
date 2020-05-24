project(core)

add_definitions(-DEXCORE_API)
earthxpert_add_library(core "api"
  SRCS "datasource/cvraster.cpp"
       "datasource/gdalraster.cpp"
       "datasource/kindfillpolicy.cpp"
       "datasource/metadata.cpp"
       "datasource/pointcloud.cpp"
       "datasource/utils.cpp"
       "math/interpcube.cpp"
       "math/interpgrid.cpp"
       "math/graphics/rotation.cpp"
       "spatialref/earthellipsoid.cpp"
       "spatialref/geocoding.cpp"
       "utils/fsutils.cpp"
       "utils/paramhelper.cpp"
       "utils/stringutils.cpp"
       "utils/layout/rasterlayout.cpp"
       "utils/resample/rasterresample.cpp"
  ITEMS ex3rd_party
        ${GDAL_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(core)
