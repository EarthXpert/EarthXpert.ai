project(core CXX)

add_definitions(-DEXCORE_API)
set(core_srcs "datasource/cvraster.cpp"
  "datasource/gdalraster.cpp"
  "datasource/kindfillpolicy.cpp"
  "datasource/metadata.cpp"
  "datasource/ogrlayer.cpp"
  "datasource/pointcloud.cpp"
  "datasource/pointcloudutils.cpp"
  "datasource/rasterutils.cpp"
  "datasource/vectorutils.cpp"
  "formats/support/supportdata.cpp"
  "formats/support/ads/adssupportdata.cpp"
  "formats/support/wv/wvsupportdata.cpp"
  "formats/support/zy3/zy3supportdata.cpp"
  "log/exlog.cpp"
  "math/interpcube.cpp"
  "math/interpgrid.cpp"
  "math/graphics/graphfit.cpp"
  "math/graphics/intersect.cpp"
  "math/graphics/rotation.cpp"
  "parallel/cuda/excuda.cpp"
  "parallel/cuda/excuda_utils.cpp"
  "parallel/intelone/exintelone.cpp"
  "parallel/omp/exomp.cpp"
  "parallel/omp/exomp_utils.cpp"
  "parallel/opencl/exopencl.cpp"
  "parallel/opencl/exopencl_utils.cpp"
  "parallel/simd/exsimd.cpp"
  "parallel/simd/exsimd_math.cpp"
  "parallel/simd/exsimd_utils.cpp"
  "spatialref/ccsct.cpp"
  "spatialref/ccssrs.cpp"
  "spatialref/ctfactory.cpp"
  "spatialref/earthellipsoid.cpp"
  "spatialref/geocoding.cpp"
  "spatialref/gdalct.cpp"
  "spatialref/gdalsrs.cpp"
  "spatialref/groundpolicy.cpp"
  "spatialref/localgeoid.cpp"
  "spatialref/srsfactory.cpp"
  "system/sysmem.cpp"
  "system/sysutils.cpp"
  "utils/fsutils.cpp"
  "utils/geomutils.cpp"
  "utils/paramhelper.cpp"
  "utils/stringutils.cpp"
  "utils/layout/layoututils.cpp"
  "utils/layout/rasterlayout.cpp"
  "utils/layout/spatiallayout.cpp"
  "utils/resample/rasterresample.cpp")
earthxpert_add_library(core "api"
  SRCS ${core_srcs}
  ITEMS ex3rdparty
        Boost::boost Boost::filesystem Boost::iostreams
        ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${CUDA_LIBRARIES} ${GDAL_LIBRARY} 
        ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} glog::glog ${LASLIB_LIBRARY}  
        ${nsimd_LIBRARIES} ${OpenCL_LIBRARIES} ${OpenCV_LIBS} ${Simd_LIBRARIES})

earthxpert_source_group(${core_srcs})
earthxpert_install_target(core)
