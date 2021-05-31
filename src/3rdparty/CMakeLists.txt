project(ex3rdparty CXX)

message(STATUS "CERES_LIBRARY: ${CERES_LIBRARY}")
message(STATUS "CGAL_LIBRARIES: ${CGAL_LIBRARIES}")
message(STATUS "exiv2_LIBRARIES: ${exiv2_LIBRARIES}")
message(STATUS "GDAL_LIBRARIES: ${GDAL_LIBRARIES}")
message(STATUS "GETTEXT_LIBRARY: ${GETTEXT_LIBRARY}")
message(STATUS "geotiff_LIBRARIES: ${geotiff_LIBRARIES}")
message(STATUS "GLIB_LIBRARIES: ${GLIB_LIBRARIES}")
message(STATUS "gsl_LIBRARIES: ${gsl_LIBRARIES}")
message(STATUS "GTEST_BOTH_LIBRARIES: ${GTEST_BOTH_LIBRARIES}")
message(STATUS "GTS_LIBRARIES: ${GTS_LIBRARIES}")
message(STATUS "ICONV_LIBRARIES: ${ICONV_LIBRARIES}")
message(STATUS "LASLIB_LIBRARY: ${LASLIB_LIBRARY}")
message(STATUS "LIBXML2_LIBRARIES: ${LIBXML2_LIBRARIES}")
message(STATUS "MPFR4_LIBRARIES: ${MPFR4_LIBRARIES}")
message(STATUS "MPIR_LIBRARIES: ${MPIR_LIBRARIES}")
message(STATUS "nsimd_LIBRARIES: ${nsimd_LIBRARIES}")
message(STATUS "OpenBLAS_LIBRARIES: ${OpenBLAS_LIBRARIES}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
message(STATUS "PCRE_LIBRARIES: ${PCRE_LIBRARIES}")
message(STATUS "PDAL_LIBRARIES: ${PDAL_LIBRARIES}")
message(STATUS "proj_LIBRARIES: ${proj_LIBRARIES}")
message(STATUS "Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")
message(STATUS "PYTHON_LIBRARIES: ${PYTHON_LIBRARIES}")
message(STATUS "Simd_LIBRARIES: ${Simd_LIBRARIES}")
message(STATUS "SLTBENCH_LIBRARIES: ${SLTBENCH_LIBRARIES}")
message(STATUS "TBB_LIBRARIES: ${TBB_LIBRARIES}")
message(STATUS "tiff_LIBRARIES: ${tiff_LIBRARIES}")
message(STATUS "vld_LIBRARIES: ${vld_LIBRARIES}")
message(STATUS "wxwidgets_LIBRARIES: ${wxwidgets_LIBRARIES}")
message(STATUS "zlib_LIBRARIES: ${zlib_LIBRARIES}")

add_definitions(-DEX3RDPARTY_API)
set(ex3rdparty_srcs 
  "blas/exblas.cpp"
  "boost/exboost.cpp"
  "ceres/bal_problem.cpp"
  "ceres/exceres.cpp"
  "cgal/excgal.cpp"
  "cgal/delaunay2d.cpp"
  "cgal/delaunay3d.cpp"
  "cuda/excuda.cpp"
  "eigen/exeigen.cpp"
  "exiv2/exexiv2.cpp"
  "gdal/exgdal.cpp"
  "geotiff/exgeotiff.cpp"
  "geotrans/exgeotrans.cpp"
  #"geotrans/exgeotrans_utils.cpp"
  "gettext/exgettext.cpp"
  "gflags/exgflags.cpp"
  "glog/exglog.cpp"
  "gnu_gsl/exgnu_gsl.cpp"
  "gsl/exgsl.cpp"
  "gtest/exgtest.cpp"
  "gts/exgts.cpp"
  "gts/delaunay2d.cpp"
  "iconv/exiconv.cpp"
  "interpolate/exinterpolate.cpp"
  "lapack/exlapack.cpp"
  "laslib/exlaslib.cpp"
  "mpfr/exmpfr.cpp"
  "mpir/exmpir.cpp"
  "nsimd/exnsimd.cpp"
  "numcpp/exnumcpp.cpp"
  "opencl/exopencl.cpp"
  "opencv/exopencv.cpp"
  "opencv/delaunay2d.cpp"
  "openimageio/exopenimageio.cpp"
  "pdal/expdal.cpp"
  "processpool/exprocesspool.cpp"
  "processpool/processpool.cpp"
  "proj/exproj.cpp"
  "protobuf/exprotobuf.cpp"
  "pybind11/expybind11.cpp"
  "qt/exqt.cpp"
  "simd/exsimd.cpp"
  "sltbench/exsltbench.cpp"
  "sqlite3/exsqlite3.cpp"
  "threadpool/exthreadpool.cpp"
  "threadpool/threadpool.cpp"
  "thrust/exthrust.cpp"
  "stl/exstl.cpp"
  "taskflow/extaskflow.cpp"
  "tbb/extbb.cpp"
  "utils/utils.cpp"
  "vld/exvld.cpp"
  "vxl/exvxl.cpp"
  "wxwidgets/exwxwidgets.cpp"
  "xml/exxml.cpp"
  "xsimd/exxsimd.cpp"
  "xtensor/exxtensor.cpp"
  "xtl/exxtl.cpp"
  "zlib/exzlib.cpp")
set(ex3rdparty_items Boost::boost Boost::date_time Boost::filesystem 
  Boost::iostreams Boost::locale Boost::numpy Boost::program_options Boost::python Boost::timer 
  ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${CUDA_LIBRARIES} ${exiv2_LIBRARIES}
  ${GDAL_LIBRARIES} ${geotiff_LIBRARIES} ${geotrans_LIBRARIES} ${GLIB_LIBRARIES} glog::glog 
  ${gsl_LIBRARIES} ${GTEST_BOTH_LIBRARIES} ${GTS_LIBRARIES} ${ICONV_LIBRARIES} ${lapack_LIBRARIES} ${LASLIB_LIBRARY} 
  libInterpolate::Interpolate ${LIBXML2_LIBRARIES} ${MPFR4_LIBRARIES} ${MPIR_LIBRARIES} 
  ${OpenBLAS_LIBRARIES} ${OpenCL_LIBRARIES} ${OpenCV_LIBS} ${OpenEXR_LIBS} 
  OpenImageIO::OpenImageIO OpenImageIO::OpenImageIO_Util 
  ${nsimd_LIBRARIES} ${PCRE_LIBRARIES} ${PDAL_LIBRARIES} ${proj_LIBRARIES} ${Protobuf_LIBRARIES} 
  ${PYTHON_LIBRARIES} Qt5::Widgets 
  ${Simd_LIBRARIES} ${SLTBENCH_LIBRARIES} ${TBB_LIBRARIES} ${tiff_LIBRARIES}
  ${wxwidgets_LIBRARIES} ${zlib_LIBRARIES})   
exai_add_library(ex3rdparty "3rdparty"
  SRCS ${ex3rdparty_srcs}
  ITEMS ${ex3rdparty_items})

exai_source_group(${ex3rdparty_srcs})
exai_install_target(ex3rdparty)
