project(core CXX)

add_definitions(-DEXCORE_API)
earthxpert_add_library(core "api"
  SRCS "datasource/cvraster.cpp"
       "datasource/gdalraster.cpp"
       "datasource/kindfillpolicy.cpp"
       "datasource/metadata.cpp"
       "datasource/ogrlayer.cpp"
       "datasource/pointcloud.cpp"
       "datasource/pointcloudutils.cpp"
       "datasource/rasterutils.cpp"
       "datasource/vectorutils.cpp"
       "math/interpcube.cpp"
       "math/interpgrid.cpp"
       "math/graphics/graphfit.cpp"
       "math/graphics/intersect.cpp"
       "math/graphics/rotation.cpp"
       "parallel/exomp.cpp"
       "spatialref/earthellipsoid.cpp"
       "spatialref/coordinatetransform.cpp"
       "spatialref/geocoding.cpp"
       "utils/fsutils.cpp"
       "utils/paramhelper.cpp"
       "utils/stringutils.cpp"
       "utils/layout/rasterlayout.cpp"
       "utils/layout/spatiallayout.cpp"
       "utils/resample/rasterresample.cpp"
  ITEMS ex3rdparty
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} glog::glog 
        ${LASLIB_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(core)
