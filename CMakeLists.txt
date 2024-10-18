# top-level CMake configuration file for EarthXpert.ai
cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
if(POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

project(earthxpert)
set(EARTHXPERTAI_VERSION 1.0.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/sdk/lib)
set(EXAI_INCLUDE_DIR "$<SHELL_PATH:${CMAKE_SOURCE_DIR}>/include")
message(STATUS "EXAI_INCLUDE_DIR: ${EXAI_INCLUDE_DIR}")
set(EXAI_WORKSPACE_DIR "$<SHELL_PATH:${CMAKE_SOURCE_DIR}>/..")
message(STATUS "EXAI_WORKSPACE_DIR: ${EXAI_WORKSPACE_DIR}")

set(EX_THIRD_PARTY "$ENV{EX_Third_Party}")

set(CUDA_ROOT "$ENV{CUDA_PATH}")
set(CUDAToolkit_ROOT ${CUDA_ROOT})
set(CUDA_BUILD_EMULATION OFF)
set(OpenCL_ROOT "$ENV{CUDA_PATH}")

option(EXAI_PACKAGES_3RDPARTY_DYNAMIC "link dynamic 3rdparty packages" ON)

#set(GFLAGS_USE_TARGET_NAMESPACE TRUE)

# section java & packages
#find_package(Java 1.8 REQUIRED Runtime Development)

# section python & packages
find_package(Python3 3.7 REQUIRED COMPONENTS Interpreter Development)
message(STATUS "Python3_EXECUTABLE: ${Python3_EXECUTABLE}")

# configuration & utils
add_subdirectory(cmake)

# section third-party
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if (MSVC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W1 /bigobj")
  endif()
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DCPL_DISABLE_DLL)

  set(EXAI_DEBUG_POSTFIX d)
  set(CDEGREL $<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>)
  message(STATUS "Debug_Release_Mode: ${CDEGREL}")

  set(EXAI_BIN_ROOT "bin")
  set(EXAI_BIN_DIR "${EXAI_BIN_ROOT}/${CDEGREL}")

  if (EXAI_PACKAGES_3RDPARTY_DYNAMIC)
    set(Boost_ROOT "${EX_THIRD_PARTY}/boost_1.71.0")
    set(Boost_DIR "${Boost_ROOT}/lib/cmake/Boost-1.71.0")
    set(Boost_INCLUDE_DIRS "${Boost_ROOT}/include")
    
    set(CEREAL_INCLUDE_DIR "${EX_THIRD_PARTY}/cereal_1.3.0/include")
    
    set(Ceres_ROOT "${EX_THIRD_PARTY}/ceres_2.0.0")
    set(Ceres_DIR "${Ceres_ROOT}/CMake")
    set(CERES_INCLUDE_DIRS "${Ceres_ROOT}/include")
    set(CERES_LIBRARY_DIR "${Ceres_ROOT}/lib")
    set(CERES_LIBRARY debug "${CERES_LIBRARY_DIR}/ceres-debug.lib"
                      optimized "${CERES_LIBRARY_DIR}/ceres.lib")
    
    set(CGAL_ROOT "${EX_THIRD_PARTY}/cgal_5.0.900")
    set(CGAL_DIR "${CGAL_ROOT}/lib/cmake/CGAL")
    set(CGAL_LIBRARIES_DIR "${CGAL_ROOT}/lib")
    set(CGAL_LIBRARIES debug "${CGAL_LIBRARIES_DIR}/CGAL_Core-vc140-mt-gd-5.0-I-900.lib"
                       debug "${CGAL_LIBRARIES_DIR}/CGAL_ImageIO-vc140-mt-gd-5.0-I-900.lib"
                       debug "${CGAL_LIBRARIES_DIR}/CGAL_Qt5-vc140-mt-gd-5.0-I-900.lib"
                       debug "${CGAL_LIBRARIES_DIR}/CGAL-vc140-mt-gd-5.0-I-900.lib"
                       optimized "${CGAL_LIBRARIES_DIR}/CGAL_Core-vc140-mt-5.0-I-900.lib"
                       optimized "${CGAL_LIBRARIES_DIR}/CGAL_ImageIO-vc140-mt-5.0-I-900.lib"
                       optimized "${CGAL_LIBRARIES_DIR}/CGAL_Qt5-vc140-mt-5.0-I-900.lib"
                       optimized "${CGAL_LIBRARIES_DIR}/CGAL-vc140-mt-5.0-I-900.lib")
    
    set(CUDA_INCLUDE_DIR "${CUDA_ROOT}/include")
    set(CUDA_BIN_DIR "${CUDA_ROOT}/bin")
    set(CUDA_LIBRARIES_DIR "${CUDA_ROOT}/lib/x64")
    set(CUDA_LIBRARIES "${CUDA_LIBRARIES_DIR}/cublas.lib"
                       "${CUDA_LIBRARIES_DIR}/cuda.lib"
                       "${CUDA_LIBRARIES_DIR}/cudart.lib"
                       "${CUDA_LIBRARIES_DIR}/cufft.lib"
                       "${CUDA_LIBRARIES_DIR}/cufftw.lib"
                       "${CUDA_LIBRARIES_DIR}/cusolver.lib"
                       "${CUDA_LIBRARIES_DIR}/cusparse.lib"
                       "${CUDA_LIBRARIES_DIR}/nvml.lib"
                       "${CUDA_LIBRARIES_DIR}/nrtc.lib")
    set(CUDAToolkit_INCLUDE_DIR ${CUDA_INCLUDE_DIR})
    set(CUDAToolkit_BIN_DIR ${CUDA_BIN_DIR})
    
    set(Eigen3_ROOT "${EX_THIRD_PARTY}/eigen_3.3.90")
    set(Eigen3_DIR "${Eigen3_ROOT}/share/eigen3/cmake")
    set(Eigen3BLAS_INCLUDE_DIR "${Eigen3_ROOT}/include")
    set(Eigen3LAPACK_INCLUDE_DIR "${Eigen3_ROOT}/include")
    set(Eigen3_LIBRARY_DIR "${Eigen3_ROOT}/lib/${CDEGREL}")
    set(Eigen3BLAS_LIBRARIES "${Eigen3_LIBRARY_DIR}/eigen_blas.lib")
    set(Eigen3LAPACK_LIBRARIES "${Eigen3_LIBRARY_DIR}/eigen_lapack.lib")
    
    set(exiv2_ROOT "${EX_THIRD_PARTY}/exiv2_0.27.3")
    set(exiv2_BIN_DIR "${exiv2_ROOT}/bin/${CDEGREL}")
    set(exiv2_DIR "${exiv2_ROOT}/share/exiv2")
    set(exiv2_INCLUDE_DIR "${exiv2_ROOT}/include")
    set(exiv2_LIBRARY_DIR "${exiv2_ROOT}/lib/${CDEGREL}")
    set(exiv2_LIBRARIES debug "${exiv2_LIBRARY_DIR}/exiv2.lib"
                        debug "${exiv2_LIBRARY_DIR}/exiv2-xmp.lib"
                        optimized "${exiv2_LIBRARY_DIR}/exiv2.lib"
                        optimized "${exiv2_LIBRARY_DIR}/exiv2-xmp.lib")
    
    set(fmt_ROOT "${EX_THIRD_PARTY}/fmt_7.1.3s")
    set(fmt_DIR "${fmt_ROOT}/share/fmt")
    set(fmt_INCLUDE_DIRS "${fmt_ROOT}/include")
    set(fmt_LIBRARY_DIR "${fmt_ROOT}/lib/${CDEGREL}")
    set(fmt_LIBRARIES debug "${fmt_LIBRARY_DIR}/fmtd.lib"
                      optimized "${fmt_LIBRARY_DIR}/fmt.lib")
    
    set(GDAL_ROOT "${EX_THIRD_PARTY}/gdal_2.4.1")
    set(GDAL_BIN_DIR "${GDAL_ROOT}/bin/${CDEGREL}")
    set(GDAL_INCLUDE_DIRS "${GDAL_ROOT}/include")
    set(GDAL_LIBRARY_DIR "${GDAL_ROOT}/lib/${CDEGREL}")
    set(GDAL_LIBRARIES debug "${GDAL_LIBRARY_DIR}/gdal_i.lib"
                       optimized "${GDAL_LIBRARY_DIR}/gdal_i.lib")
    
    set(Geogram_ROOT "${EX_THIRD_PARTY}/geogram_1.7.5")
    set(Geogram_BIN_DIR "${Geogram_ROOT}/bin/${CDEGREL}")
    set(Geogram_INCLUDE_DIR "${Geogram_ROOT}/include")
    set(Geogram_LIBRARY_DIR "${Geogram_ROOT}/lib/${CDEGREL}")
    set(Geogram_LIBRARIES debug "${Geogram_LIBRARY_DIR}/geogram.lib"
                          debug "${Geogram_LIBRARY_DIR}/geogram_num_3rdparty.lib"
                          optimized "${Geogram_LIBRARY_DIR}/geogram.lib"
                          optimized "${Geogram_LIBRARY_DIR}/geogram_num_3rdparty.lib")
    
    set(geos_ROOT "${EX_THIRD_PARTY}/geos_3.9.0")
    set(geos_BIN_DIR "${geos_ROOT}/bin/${CDEGREL}")
    set(geos_INCLUDE_DIR "${geos_ROOT}/include")
    set(geos_LIBRARY_DIR "${geos_ROOT}/lib/${CDEGREL}")
    set(geos_LIBRARIES debug "${geos_LIBRARY_DIR}/geos_cd.lib"
                       debug "${geos_LIBRARY_DIR}/geosd.lib"
                       optimized "${geos_LIBRARY_DIR}/geos.lib"
                       optimized "${geos_LIBRARY_DIR}/geos_c.lib")
    
    set(geotiff_ROOT "${EX_THIRD_PARTY}/libgeotiff_1.4.2")
    set(geotiff_BIN_DIR "${geotiff_ROOT}/bin/${CDEGREL}")
    set(geotiff_DIR "${geotiff_ROOT}/share/libgeotiff")
    set(geotiff_INCLUDE_DIR "${geotiff_ROOT}/include")
    set(geotiff_LIBRARY_DIR "${geotiff_ROOT}/lib/${CDEGREL}")
    set(geotiff_LIBRARIES debug "${geotiff_LIBRARY_DIR}/geotiff_d_i.lib"
                          optimized "${geotiff_LIBRARY_DIR}/geotiff_i.lib")
    
    set(geotrans_ROOT "${EX_THIRD_PARTY}/geotrans_3.8.0")
    set(geotrans_BIN_DIR "${geotrans_ROOT}/bin/${CDEGREL}")
    set(geotrans_INCLUDE_DIR "${geotrans_ROOT}/include")
    set(geotrans_LIBRARY_DIR "${geotrans_ROOT}/lib/${CDEGREL}")
    set(geotrans_LIBRARIES "${geotrans_LIBRARY_DIR}/MSPCoordinateConversionService.lib"
                           "${geotrans_LIBRARY_DIR}/MSPdtcc.lib")
    
    set(GETTEXT_ROOT "${EX_THIRD_PARTY}/gettext_0.19.0")
    set(GETTEXT_BIN_DIR "${GETTEXT_ROOT}/bin/${CDEGREL}")
    set(GETTEXT_INCLUDE_DIR "${GETTEXT_ROOT}/include")
    set(GETTEXT_LIBRARY "${GETTEXT_ROOT}/lib/${CDEGREL}/libintl.lib")
    
    #set(GFLAGS_ROOT "${EX_THIRD_PARTY}/gflags_2.2.2")
    #set(gflags_DIR "${GFLAGS_ROOT}/lib/cmake/gflags")
    #set(GFLAGS_INCLUDE_DIR "${GFLAGS_ROOT}/include")

    set(GLIB_ROOT "${EX_THIRD_PARTY}/glib_2.52.3")
    set(GLIB_BIN_DIR "${GLIB_ROOT}/bin/${CDEGREL}")
    set(GLIB_INCLUDE_DIR "${GLIB_ROOT}/include")
    set(GLIB_LIBRARY_DIR "${GLIB_ROOT}/lib/${CDEGREL}")
    set(GLIB_LIBRARIES "${GLIB_LIBRARY_DIR}/gio-2.0.lib"
                       "${GLIB_LIBRARY_DIR}/glib-2.0.lib"
                       "${GLIB_LIBRARY_DIR}/gmodule-2.0.lib"
                       "${GLIB_LIBRARY_DIR}/gobject-2.0.lib"
                       "${GLIB_LIBRARY_DIR}/gthread-2.0.lib")
    
    set(GLOG_ROOT "${EX_THIRD_PARTY}/glog_0.4.0")
    set(glog_DIR "${GLOG_ROOT}/lib/cmake/glog")
    set(GLOG_INCLUDE_DIR "${GLOG_ROOT}/include")
    set(GLOG_LIBRARY_DIR "${GLOG_ROOT}/lib")
    set(GLOG_LIBRARY debug "${GLOG_LIBRARY_DIR}/glogd.lib"
                     optimized "${GLOG_LIBRARY_DIR}/glog.lib")
    
    #GMT_DIR
    #GMTSAR_DIR
    #gsl core guideline
    
    set(gsl_ROOT "${EX_THIRD_PARTY}/gsl_2.4.0")     #gnu_gsl
    set(gsl_BIN_DIR "${gsl_ROOT}/bin/${CDEGREL}")
    set(gsl_INCLUDE_DIR "${gsl_ROOT}/include")
    set(gsl_LIBRARY_DIR "${gsl_ROOT}/lib/${CDEGREL}")
    set(gsl_LIBRARIES debug "${gsl_LIBRARY_DIR}/gslcblasd.lib"
                      debug "${gsl_LIBRARY_DIR}/gsld.lib"
                      optimized "${gsl_LIBRARY_DIR}/gslcblas.lib"
                      optimized "${gsl_LIBRARY_DIR}/gsl.lib")
    
    set(GTEST_ROOT "${EX_THIRD_PARTY}/gtest_1.8.1")
    set(GTEST_BIN_DIR "${GTEST_ROOT}/bin")
    set(GTEST_INCLUDE_DIR "${GTEST_ROOT}/include")

    set(GTS_ROOT "${EX_THIRD_PARTY}/gts_0.7.6")
    set(GTS_BIN_DIR "${GTS_ROOT}/bin/${CDEGREL}")
    set(GTS_INCLUDE_DIR "${GTS_ROOT}/include")
    set(GTS_LIBRARY_DIR "${GTS_ROOT}/lib/${CDEGREL}")
    set(GTS_LIBRARIES "${GTS_LIBRARY_DIR}/gts.lib")
    
    set(HDF5_ROOT "${EX_THIRD_PARTY}/hdf5_1.10.5")
    set(HDF5_BIN_DIR "${HDF5_ROOT}/bin")
    
    set(ICONV_ROOT "${EX_THIRD_PARTY}/iconv_1.16.0")
    set(ICONV_DIR "${ICONV_ROOT}/share/iconv/CMake")
    set(ICONV_INCLUDE_DIRS "${ICONV_ROOT}/include")
    set(ICONV_LIBRARY_DIR "${ICONV_ROOT}/lib/${CDEGREL}")
    set(ICONV_LIBRARIES "${ICONV_LIBRARY_DIR}/iconv.lib")
    
    set(libInterpolate_DIR "${EX_THIRD_PARTY}/libInterpolate_2.3.2/cmake")
    
    #set(ITK_ROOT)

    set(lapack_ROOT "${EX_THIRD_PARTY}/lapack_3.9.0")
    set(lapack_DIR "${lapack_ROOT}/lib/cmake")
    set(lapack_BIN_DIR "${lapck_ROOT}/bin/${CDEGREL}")
    set(lapack_INCLUDE_DIR "${lapack_ROOT}/include")
    set(lapack_LIBRARY_DIR "${lapack_ROOT}/lib/${CDEGREL}")
    set(lapack_LIBRARIES "${lapack_LIBRARY_DIR}/libblas.lib"
                         "${lapack_LIBRARY_DIR}/libcblas.lib"
                         "${lapack_LIBRARY_DIR}/liblapack.lib"
                         "${lapack_LIBRARY_DIR}/liblapacke.lib"
                         "${lapack_LIBRARY_DIR}/libtmglib.lib")
    
    set(LASLIB_ROOT "${EX_THIRD_PARTY}/lastools_19.05.07")
    #set(LASLIB_BIN_DIR "${LASLIB_ROOT}/bin")
    set(LASLIB_INCLUDE_DIR "${LASLIB_ROOT}/include/LASlib")
    set(LASLIB_LIBRARY "${LASLIB_ROOT}/lib/${CDEGREL}/LASlib/LASlib.lib")
    
    set(magic_enum_ROOT "${EX_THIRD_PARTY}/magic_enum_0.7.3")
    set(magic_enum_DIR "${magic_enum_ROOT}/share/magic_enum")

    set(matplotlib_ROOT "${EX_THIRD_PARTY}/matplotlib-cpp_21.06.09")
    #set(matplotlib_DIR "${matplotlib_ROOT}/share")
    set(matplotlib_INCLUDE_DIR "${matplotlib_ROOT}/include")
 
    set(MPFR4_ROOT "${EX_THIRD_PARTY}/mpfr_4.0.2")
    #set(MPFR4_DIR "${MPFR4_ROOT}/share/mpfr")
    set(MPFR4_INCLUDE_DIR "${MPFR4_ROOT}/include")
    set(MPFR4_LIBRARY_DIR "${MPFR4_ROOT}/lib/${CDEGREL}")
    set(MPFR4_LIBRARIES "${MPFR4_LIBRARY_DIR}/mpfr.lib")
    
    set(MPIR_ROOT "${EX_THIRD_PARTY}/mpir_3.0.0")
    #set(MPIR_DIR "${MPIR_ROOT}/share/mpir")
    set(MPIR_INCLUDE_DIR "${MPIR_ROOT}/include")
    set(MPIR_LIBRARY_DIR "${MPIR_ROOT}/lib/${CDEGREL}")
    set(MPIR_LIBRARIES "${MPIR_LIBRARY_DIR}/mpir.lib")
    
    set(nlohmann_json_ROOT "${EX_THIRD_PARTY}/nlohmann-json_3.7.3")
    set(nlohmann_json_DIR "${nlohmann_json_ROOT}/share/nlohmann_json")
    set(nlohmann_json_INCLUDE_DIR "${nlohmann_json_ROOT}/include")
    
    set(nsimd_ROOT "${EX_THIRD_PARTY}/nsimd_20.11.01")
    #set(nsimd_DIR "${nsimd_ROOT}/lib/cmake")
    set(nsimd_BIN_DIR "${nsimd_ROOT}/bin/${CDEGREL}")
    set(nsimd_INCLUDE_DIR "${nsimd_ROOT}/include")
    set(nsimd_LIBRARY_DIR "${nsimd_ROOT}/lib/${CDEGREL}")
    set(nsimd_LIBRARIES "${nsimd_LIBRARY_DIR}/libnsimd_x86_64.lib")
    
    set(NumCpp_ROOT "${EX_THIRD_PARTY}/numcpp_2.3.1")
    set(NumCpp_DIR "${NumCpp_ROOT}/share\/NumCpp/cmake")
    
    set(OpenBLAS_ROOT "${EX_THIRD_PARTY}/openblas_0.3.7")
    set(OpenBLAS_DIR "${OpenBLAS_ROOT}/share/openblas")
    set(OpenBLAS_BIN_DIR "${OpenBLAS_ROOT}/bin/${CDEGREL}")
    set(OpenBLAS_INCLUDE_DIR "${OpenBLAS_ROOT}/include")
    set(OpenBLAS_LIBRARY_DIR "${OpenBLAS_ROOT}/lib/${CDEGREL}")
    set(OpenBLAS_LIBRARIES "${OpenBLAS_LIBRARY_DIR}/openblas.lib")
    
    set(OpenCL_INCLUDE_DIRS "${OpenCL_ROOT}/include")
    set(OpenCL_BIN_DIR "${OpenCL_ROOT}/bin")
    set(OpenCL_LIBRARY_DIR "${OpenCL_ROOT}/lib/x64")
    set(OpenCL_LIBRARIES "${OpenCL_LIBRARY_DIR}/OpenCL.lib")
    
    set(OpenCV_ROOT "${EX_THIRD_PARTY}/opencv_4.1.0")
    set(OpenCV_DIR "${OpenCV_ROOT}/x64/vc15/lib")
    set(OpenCV_BIN_DIR "${OpenCV_ROOT}/x64/vc15/bin")
    set(OpenCV_INCLUDE_DIR "${OpenCV_ROOT}/include")
    set(OpenCV_LIBRARY_DIR "${OpenCV_ROOT}/x64/vc15/lib")
    set(OpenCV_LIBS debug "${OpenCV_LIBRARY_DIR}/opencv_img_hash410d.lib"
                    debug "${OpenCV_LIBRARY_DIR}/opencv_world410d.lib"
                    optimized "${OpenCV_LIBRARY_DIR}/opencv_img_hash410.lib"
                    optimized "${OpenCV_LIBRARY_DIR}/opencv_world410.lib")
    
    set(OpenEXR_ROOT "${EX_THIRD_PARTY}/openexr_2.3.0")
    set(OpenEXR_DIR "${OpenEXR_ROOT}/share/openexr")
    set(OpenEXR_INCLUDE_DIR "${OpenEXR_ROOT}/include")
    set(OpenEXR_LIBRARY_DIR "${OpenEXR_ROOT}/lib/${CDEGREL}")
    set(OpenEXR_LIBS debug "${OpenEXR_LIBRARY_DIR}/Half-2_3_d.lib"
                     debug "${OpenEXR_LIBRARY_DIR}/Iex-2_3_d.lib"
                     debug "${OpenEXR_LIBRARY_DIR}/IexMath-2_3_d.lib"
                     debug "${OpenEXR_LIBRARY_DIR}/IlmImf-2_3_d.lib"
                     debug "${OpenEXR_LIBRARY_DIR}/IlmImfUtil-2_3_d.lib"
                     debug "${OpenEXR_LIBRARY_DIR}/IlmThread-2_3_d.lib"
                     debug "${OpenEXR_LIBRARY_DIR}/Imath-2_3_d.lib"
                     optimized "${OpenEXR_LIBRARY_DIR}/Half-2_3.lib"
                     optimized "${OpenEXR_LIBRARY_DIR}/Iex-2_3.lib"
                     optimized "${OpenEXR_LIBRARY_DIR}/IexMath-2_3.lib"
                     optimized "${OpenEXR_LIBRARY_DIR}/IlmImf-2_3.lib"
                     optimized "${OpenEXR_LIBRARY_DIR}/IlmImfUtil-2_3.lib"
                     optimized "${OpenEXR_LIBRARY_DIR}/IlmThread-2_3.lib"
                     optimized "${OpenEXR_LIBRARY_DIR}/Imath-2_3.lib")
    
    set(OpenImageIO_ROOT "${EX_THIRD_PARTY}/openimageio_2.1.9")
    set(OpenImageIO_DIR "${OpenImageIO_ROOT}/share/openimageio")
    set(OpenImageIO_INCLUDE_DIR "${OpenImageIO_ROOT}/include")
    
    set(OPENNI_ROOT "${EX_THIRD_PARTY}/openni_2.2.0")
    set(OPENNI_BIN_DIR "${OPENNI_ROOT}/Redist")
  
    #set(OTB_ROOT)

    set(PCRE_ROOT "${EX_THIRD_PARTY}/pcre_8.44.0")
    set(PCRE_BIN_DIR "${PCRE_ROOT}/bin/${CDEGREL}")
    set(PCRE_INCLUDE_DIR "${PCRE_ROOT}/include")
    set(PCRE_LIBRARY_DIR "${PCRE_ROOT}/lib/${CDEGREL}")
    set(PCRE_LIBRARIES debug "${PCRE_LIBRARY_DIR}/pcred.lib"
                       debug "${PCRE_LIBRARY_DIR}/pcre16d.lib"
                       debug "${PCRE_LIBRARY_DIR}/pcre32d.lib"
                       debug "${PCRE_LIBRARY_DIR}/pcrecppd.lib"
                       debug "${PCRE_LIBRARY_DIR}/pcreposixd.lib"
                       optimized "${PCRE_LIBRARY_DIR}/pcre.lib"
                       optimized "${PCRE_LIBRARY_DIR}/pcre16.lib"
                       optimized "${PCRE_LIBRARY_DIR}/pcre32.lib"
                       optimized "${PCRE_LIBRARY_DIR}/pcrecpp.lib"
                       optimized "${PCRE_LIBRARY_DIR}/pcreposix.lib")

    set(PDAL_ROOT "${EX_THIRD_PARTY}/pdal_1.9.1")
    set(PDAL_DIR "${PDAL_ROOT}/lib/cmake/PDAL")
    
    set(proj_ROOT "${EX_THIRD_PARTY}/proj_8.0.0")
    #set(proj_DIR "${proj_ROOT}/lib/cmake/proj")
    set(proj_BIN_DIR "${proj_ROOT}/bin/${CDEGREL}")
    set(proj_INCLUDE_DIR "${proj_ROOT}/include")
    set(proj_LIBRARY_DIR "${proj_ROOT}/lib/${CDEGREL}")
    set(proj_LIBRARIES debug "${proj_LIBRARY_DIR}/proj_d.lib"
                       optimized "${proj_LIBRARY_DIR}/proj.lib")
    
    set(protobuf_ROOT "${EX_THIRD_PARTY}/protobuf_3.11.3")
    set(protobuf_DIR "${protobuf_ROOT}/share/protobuf")
    set(Protobuf_INCLUDE_DIRS "${protobuf_ROOT}/include" 
                              "${protobuf_ROOT}/include/google")
    set(Protobuf_LIBRARY_DIR "${protobuf_ROOT}/lib/${CDEGREL}")
    set(Protobuf_LIBRARIES debug "${Protobuf_LIBRARY_DIR}/libprotobufd.lib"
                           debug "${Protobuf_LIBRARY_DIR}/libprotobuf-lited.lib"
                           debug "${Protobuf_LIBRARY_DIR}/libprotocd.lib"
                           optimized "${Protobuf_LIBRARY_DIR}/libprotobuf.lib"
                           optimized "${Protobuf_LIBRARY_DIR}/libprotobuf-lite.lib"
                           optimized "${Protobuf_LIBRARY_DIR}/libprotoc.lib")
    
    set(pybind11_DIR "${EX_THIRD_PARTY}/pybind11_2.5.0/share/cmake/pybind11")
    
    #set(PYTHON_ROOT_DIR "python37")
    
    set(Qt5_ROOT "${EX_THIRD_PARTY}/qt_5.11.2")
    set(Qt5_DIR "${Qt5_ROOT}/lib/cmake/Qt5")
    set(Qt5LinguistTools_DIR "${Qt5_ROOT}/lib/cmake/Qt5LinguistTools")
    
    set(range-v3_ROOT "${EX_THIRD_PARTY}/range-v3_0.9.0")
    set(range-v3_DIR "${range-v3_ROOT}/lib/cmake/range-v3")
    
    set(Simd_ROOT "${EX_THIRD_PARTY}/Simd_4.6.94")
    #set(Simd_DIR "${Simd_ROOT}/lib/cmake")
    set(Simd_BIN_DIR "${Simd_ROOT}/bin/${CDEGREL}")
    set(Simd_INCLUDE_DIR "${Simd_ROOT}/include")
    set(Simd_LIBRARY_DIR "${Simd_ROOT}/lib")
    set(Simd_LIBRARIES "${Simd_LIBRARY_DIR}/${CDEGREL}/Simd.lib")
    
    set(Sltbench_DIR "${EX_THIRD_PARTY}/sltbench_20.01.15/cmake")
    
    #SNAPHU_DIR
    
    set(sqlite3_ROOT "${EX_THIRD_PARTY}/sqlite_3.31.1")
    set(sqlite3_DIR "${EX_THIRD_PARTY}/share/sqlite3")
    
    set(Taskflow_ROOT "${EX_THIRD_PARTY}/taskflow_3.1.0")
    set(Taskflow_DIR "${Taskflow_ROOT}/lib/cmake/Taskflow")
    
    set(TBB_ROOT "${EX_THIRD_PARTY}/tbb_20.0.0")
    set(TBB_DIR "${TBB_ROOT}/share/tbb")
    set(TBB_INCLUDE_DIRS "${TBB_ROOT}/include")
    set(TBB_LIBRARY_DIR "${TBB_ROOT}/lib/${CDEGREL}")
    set(TBB_LIBRARIES debug "${TBB_LIBRARY_DIR}/tbb_debug.lib"
                      debug "${TBB_LIBRARY_DIR}/tbbmalloc_debug.lib"
                      debug "${TBB_LIBRARY_DIR}/tbbmalloc_proxy_debug.lib"
                      optimized "${TBB_LIBRARY_DIR}/tbb.lib"
                      optimized "${TBB_LIBRARY_DIR}/tbbmalloc.lib"
                      optimized "${TBB_LIBRARY_DIR}/tbbmalloc_proxy.lib")
    
    set(tiff_ROOT "${EX_THIRD_PARTY}/tiff_4.0.10")
    set(tiff_BIN_DIR "${tiff_ROOT}/bin/${CDEGREL}")
    #set(tiff_DIR "${tiff_ROOT}/share/tiff")
    set(tiff_INCLUDE_DIR "${tiff_ROOT}/include")
    set(tiff_LIBRARY_DIR "${tiff_ROOT}/lib/${CDEGREL}")
    set(tiff_LIBRARIES debug "${tiff_LIBRARY_DIR}/tiffd.lib"
                       debug "${tiff_LIBRARY_DIR}/tiffxxd.lib"
                       optimized "${tiff_LIBRARY_DIR}/tiff.lib"
                       optimized "${tiff_LIBRARY_DIR}/tiffxx.lib")

    set(tinyply_ROOT "${EX_THIRD_PARTY}/tinyply_2.3.3")
    set(tinyply_DIR "${tinyply_ROOT}/share/tinyply")
    
    set(vld_ROOT "${EX_THIRD_PARTY}/vld_2.5.1")
    #set(vld_DIR "${vld_ROOT}/lib/cmake")
    set(vld_BIN_DIR "${vld_ROOT}/bin")
    set(vld_INCLUDE_DIR "${vld_ROOT}/include")
    set(vld_LIBRARY_DIR "${vld_ROOT}/lib")
    set(vld_LIBRARIES debug "${vld_LIBRARY_DIR}/vld.lib")
    
    set(wxwidgets_ROOT "${EX_THIRD_PARTY}/wxwidgets_3.1.4")
    set(wxwidgets_INCLUDE_DIRS "${wxwidgets_ROOT}/include"
                               "${wxwidgets_ROOT}/include/msvc")
    set(wxwidgets_LIBRARY_DIR "${wxwidgets_ROOT}/lib/${CDEGREL}")
    set(wxwidgets_LIBRARIES debug "${wxwidgets_LIBRARY_DIR}/wxbase31ud.lib"
                            debug "${wxwidgets_LIBRARY_DIR}/wxmsw31ud_core.lib"
                            optimized "${wxwidgets_LIBRARY_DIR}/wxbase31u.lib"
                            optimized "${wxwidgets_LIBRARY_DIR}/wxmsw31u_core.lib")
     
    set(LIBXML2_ROOT "${EX_THIRD_PARTY}/xml2_2.9.9")
    set(LIBXML2_DIR "${LIBXML2_ROOT}/share/libxml2/CMake")
    set(LIBXML2_INCLUDE_DIRS "${LIBXML2_ROOT}/include")
    set(LIBXML2_LIBRARY_DIR "${LIBXML2_ROOT}/lib/${CDEGREL}")
    set(LIBXML2_LIBRARIES "${LIBXML2_LIBRARY_DIR}/libxml2.lib")
   
    set(xsimd_DIR "${EX_THIRD_PARTY}/xsimd_7.4.6/share/xsimd")
    
    set(xtensor_ROOT "${EX_THIRD_PARTY}/xtensor_0.21.3")
    set(xtensor_DIR "${xtensor_ROOT}/share/xtensor")
    
    set(xtensor-python_ROOT "${EX_THIRD_PARTY}/xtensor-python_0.24.1")
    set(xtensor-python_DIR "${xtensor_ROOT}/lib/cmake/xtensor-python")
    
    set(xtl_ROOT "${EX_THIRD_PARTY}/xtl_0.6.11")
    set(xtl_DIR "${xtl_ROOT}/share/xtl")
    
    set(yaml_ROOT "${EX_THIRD_PARTY}/libyaml_0.2.5")
    set(yaml_DIR "${yaml_ROOT}/share/yaml")
    
    set(zlib_ROOT "${EX_THIRD_PARTY}/zlib_1.2.11s")
    set(zlib_INCLUDE_DIR "${zlib_ROOT}/include")
    set(zlib_LIBRARY_DIR "${zlib_ROOT}/lib")  #/${CDEGREL}
    set(zlib_LIBRARIES debug "${zlib_LIBRARY_DIR}/zlibd.lib"
                       optimized "${zlib_LIBRARY_DIR}/zlib.lib")
  else()

    set(Ceres_ROOT "${EX_THIRD_PARTY}/ceres_2.0.0s")
    set(Ceres_DIR "${Ceres_ROOT}/CMake")
    set(CERES_INCLUDE_DIRS "${Ceres_ROOT}/include")
    set(CERES_LIBRARY_DIR "${Ceres_ROOT}/lib")
    set(CERES_LIBRARY debug "${CERES_LIBRARY_DIR}/ceres-debug.lib"
                      optimized "${CERES_LIBRARY_DIR}/ceres.lib")

    set(Eigen3_ROOT "${EX_THIRD_PARTY}/eigen_3.3.9")
    set(Eigen3_DIR "${Eigen3_ROOT}/share/eigen3/cmake")

    set(GDAL_ROOT "${EX_THIRD_PARTY}/gdal_3.1.3s")
    set(GDAL_INCLUDE_DIRS "${GDAL_ROOT}/include")
    set(GDAL_LIBRARIES debug "${GDAL_ROOT}/lib/gdal_d.lib"
                       optimized "${GDAL_ROOT}/lib/gdal.lib")
    
    set(OpenCV_ROOT "${EX_THIRD_PARTY}/opencv_4.5.1s")
    set(OpenCV_DIR "${OpenCV_ROOT}/share/opencv")
    set(OpenCV_INCLUDE_DIR "${OpenCV_ROOT}/include")
    set(OpenCV_LIBRARY_DIR "${OpenCV_ROOT}/lib")
    set(OpenCV_LIBS debug "${OpenCV_LIBRARY_DIR}/opencv_cored.lib"
                    debug "${OpenCV_LIBRARY_DIR}/opencv_imgprocd.lib"
                    optimized "${OpenCV_LIBRARY_DIR}/opencv_core.lib"
                    optimized "${OpenCV_LIBRARY_DIR}/opencv_imgproc.lib")

  endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

  set(EXAI_BIN_ROOT "bin")
  set(EXAI_BIN_DIR "bin")
  set(EXAI_DEBUG_POSTFIX "")

  set(CEREAL_INCLUDE_DIR "${EX_THIRD_PARTY}/cereal_1.2.2/include")

  set(GDAL_ROOT "${EX_THIRD_PARTY}/gdal_2.3.2")
  set(GDAL_INCLUDE_DIRS "${GDAL_ROOT}/include")
  set(GDAL_LIBRARIES "${GDAL_ROOT}/lib/libgdal.so")

  set(GTEST_ROOT "${EX_THIRD_PARTY}/gtest_1.8.1")
  set(GTEST_INCLUDE_DIR "${GTEST_ROOT}/include")
  
  set(LASLIB_ROOT "${EX_THIRD_PARTY}/lastools_19.01.27")
  set(LASLIB_INCLUDE_DIR "${LASLIB_ROOT}/include/LASlib")
  set(LASLIB_LIBRARY "${LASLIB_ROOT}/lib/LASlib/libLASlib.a")

  set(OpenCV_ROOT "${EX_THIRD_PARTY}/opencv_4.1.0")
  set(OpenCV_DIR "${OpenCV_ROOT}/lib/cmake/opencv4")
  set(OpenCV_INCLUDE_DIR "${OpenCV_ROOT}/include/opencv4")
  set(OpenCV_LIBRARY_DIR "${OpenCV_ROOT}/lib")
  set(OpenCV_LIBS "${OpenCV_LIBRARY_DIR}/libopencv_img_hash.so"
                  "${OpenCV_LIBRARY_DIR}/libopencv_sfm.so"
                  "${OpenCV_LIBRARY_DIR}/libopencv_world.so")
                  
else()
  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME}: not supported.")
endif()

message(STATUS "EXAI_BIN_DIR: ${EXAI_BIN_DIR}")

if(DEFINED CMAKE_DEBUG_POSTFIX)
  set(EXAI_DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.71.0 COMPONENTS date_time filesystem fusion graph hana iostreams locale math mpl numpy37 program_options python37 system timer) 

find_package(Ceres CONFIG REQUIRED)  # ceres Ceres::ceres
find_package(CGAL CONFIG REQUIRED)   # CGAL::CGAL
find_package(CUDA REQUIRED)
find_package(CUDAToolkit REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(exiv2 CONFIG REQUIRED) # exiv2lib exiv2-xmp
find_package(fmt CONFIG REQUIRED)
#find_package(GDAL REQUIRED)
#find_package(Geogram CONFIG REQUIRED)
#find_package(geos CONFIG REQUIRED)
#find_package(geotiff CONFIG REQUIRED)
#find_package(geotrans CONFIG REQUIRED)
#find_package(GETTEXT CONFIG REQUIRED)
#find_package(gflags CONFIG REQUIRED)
#find_package(glib REQUIRED)
find_package(glog CONFIG REQUIRED)
#find_package(gsl CONFIG REQUIRED)
find_package(GTest REQUIRED)
#find_package(GTS REQUIRED)
find_package(ICONV CONFIG REQUIRED)
find_package(libInterpolate CONFIG REQUIRED)
#find_package(ITK CONFIG REQUIRED)
#find_package(lapack CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
#find_package(MPFR CONFIG REQUIRED)
#find_package(MPIR CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
#find_package(nsimd CONFIG REQUIRED)
find_package(NumCpp CONFIG REQUIRED)
#find_package(OpenBLAS CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)   # opencv_ml opencv_dnn opencv_core opencv_flann etc
#find_package(OpenCL CONFIG REQUIRED)
#find_package(OpenEXR CONFIG REQUIRED)
find_package(OpenImageIO CONFIG REQUIRED)
#find_package(OTB CONFIG REQUIRED)
#find_package(pcre REQUIRED)
find_package(PDAL CONFIG REQUIRED)
#find_package(proj CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(Qt5 5.11.2 COMPONENTS Core Gui LinguistTools OpenGL Widgets REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
#find_package(Simd REQUIRED)
find_package(sqlite3 CONFIG REQUIRED)
find_package(Sltbench CONFIG REQUIRED)
find_package(Taskflow CONFIG REQUIRED)
#find_package(TBB CONFIG REQUIRED)
#find_package(tiff CONFIG REQUIRED)
find_package(tinyply CONFIG REQUIRED)
#find_package(wxwidgets CONFIG REQUIRED)
#find_package(vld)
find_package(LIBXML2 CONFIG REQUIRED)
find_package(xsimd CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)
find_package(xtensor-python CONFIG REQUIRED)
find_package(xtl CONFIG REQUIRED)
find_package(yaml CONFIG REQUIRED)
#find_package(zlib REQUIRED)

# remark: BOOST_BIN_DIR is empty in subdirectory!
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(Boost_INCLUDE_DIRS "${Boost_ROOT}/include")
  set(BOOST_BIN_DIR "${Boost_DIR}/../..")
  message(STATUS "BOOST_BIN_DIR: ${BOOST_BIN_DIR}")
else()

endif()

#exai_get_INCLUDE_DIRS(libInterpolate libInterpolate::Interpolate)
#exai_get_INCLUDE_DIRS(SLTBENCH sltbench)
#exai_get_LIBRARIES(SLTBENCH sltbench)

get_target_property(libInterpolate_INCLUDE_DIRS libInterpolate::Interpolate INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(range-v3_INCLUDE_DIRS range-v3 INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SLTBENCH_INCLUDE_DIRS sltbench INTERFACE_INCLUDE_DIRECTORIES)

get_target_property(SLTBENCH_LIBRARY_DEBUG sltbench IMPORTED_LOCATION_DEBUG)
get_target_property(SLTBENCH_LIBRARY_RELEASE sltbench IMPORTED_LOCATION_RELEASE)
set(SLTBENCH_LIBRARIES debug ${SLTBENCH_LIBRARY_DEBUG}
                       optimized ${SLTBENCH_LIBRARY_RELEASE})

# python packages
set(PYTHON_ROOT "${Python3_STDLIB}/..") 
set(PYTHON_LIBRARY_DIRS "${PYTHON_ROOT}/libs")
set(PYTHON_SITELIB_DIR "${Python3_SITELIB}")
set(PYTHON_CUPY_ROOT "${PYTHON_SITELIB_DIR}/cupy")
set(PYTHON_LXML_ROOT "${PYTHON_SITELIB_DIR}/lxml")

set(PYTHON_NUMPY_ROOT "${PYTHON_SITELIB_DIR}/numpy")
set(PYTHON_NUMPY_INCLUDE_DIRS "${PYTHON_NUMPY_ROOT}/core/include")
set(PYTHON_NUMPY_LIBRARY_DIR "${PYTHON_NUMPY_ROOT}/core/lib")
set(PYTHON_NUMPY_LIBRARIES "${PYTHON_NUMPY_LIBRARY_DIR}/npymath.lib")

set(PYTHON_OSGEO_ROOT "${PYTHON_SITELIB_DIR}/osgeo")
#set(PYTHON_SCIPY_ROOT "")
set(PYTHON_TENSORFLOW_ROOT "${PYTHON_SITELIB_DIR}/tensorflow_core")
set(PYTHON_TORCH_ROOT "${PYTHON_SITELIB_DIR}/torch")

# section include
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "CEREAL_INCLUDE_DIR: ${CEREAL_INCLUDE_DIR}")
message(STATUS "CERES_INCLUDE_DIRS: ${CERES_INCLUDE_DIRS}")
message(STATUS "CGAL_INCLUDE_DIRS: ${CGAL_INCLUDE_DIRS}")
message(STATUS "CUDA_INCLUDE_DIR: ${CUDA_INCLUDE_DIR}")
message(STATUS "EIGEN3_INCLUDE_DIR: ${EIGEN3_INCLUDE_DIR}")
message(STATUS "exiv2_INCLUDE_DIR: ${exiv2_INCLUDE_DIR}")
message(STATUS "fmt_INCLUDE_DIRS: ${fmt_INCLUDE_DIRS}")
message(STATUS "GDAL_INCLUDE_DIRS: ${GDAL_INCLUDE_DIRS}")
message(STATUS "Geogram_INCLUDE_DIR: ${Geogram_INCLUDE_DIR}")
message(STATUS "geos_INCLUDE_DIR: ${geos_INCLUDE_DIR}")
message(STATUS "geotiff_INCLUDE_DIR: ${geotiff_INCLUDE_DIR}")
message(STATUS "geotrans_INCLUDE_DIR: ${geotrans_INCLUDE_DIR}")
message(STATUS "GETTEXT_INCLUDE_DIR: ${GDAL_INCLUDE_DIRS}")
#message(STATUS "GFLAGS_INCLUDE_DIR: ${GLOG_INCLUDE_DIR}")
message(STATUS "GLIB_INCLUDE_DIR: ${GLIB_INCLUDE_DIR}")
message(STATUS "GLOG_INCLUDE_DIR: ${GLOG_INCLUDE_DIR}")
message(STATUS "gsl_INCLUDE_DIR: ${gsl_INCLUDE_DIR}")
message(STATUS "GTEST_INCLUDE_DIRS: ${GTEST_INCLUDE_DIRS}")
message(STATUS "GTS_INCLUDE_DIR: ${GTS_INCLUDE_DIR}")
message(STATUS "ICONV_INCLUDE_DIRS: ${ICONV_INCLUDE_DIRS}")
message(STATUS "libInterpolate_INCLUDE_DIRS: ${libInterpolate_INCLUDE_DIRS}")
message(STATUS "lapack_INCLUDE_DIR: ${lapack_INCLUDE_DIR}")
message(STATUS "LASLIB_INCLUDE_DIR: ${LASLIB_INCLUDE_DIR}")
message(STATUS "MPFR_INCLUDE_DIR: ${MPFR_INCLUDE_DIR}")
message(STATUS "MPFR4_INCLUDE_DIR: ${MPFR4_INCLUDE_DIR}")
message(STATUS "MPIR_INCLUDE_DIR: ${MPIR_INCLUDE_DIR}")
message(STATUS "nlohmann_json_INCLUDE_DIR: ${nlohmann_json_INCLUDE_DIR}")
message(STATUS "nsimd_INCLUDE_DIR: ${nsimd_INCLUDE_DIR}")
message(STATUS "NumCpp_INCLUDE_DIR: ${NumCpp_INCLUDE_DIR}")
message(STATUS "OpenBLAS_INCLUDE_DIR: ${OpenBLAS_INCLUDE_DIR}")
message(STATUS "OpenCL_INCLUDE_DIRS: ${OpenCL_INCLUDE_DIRS}")
message(STATUS "OpenCV_INCLUDE_DIR: ${OpenCV_INCLUDE_DIR}")
message(STATUS "OpenEXR_INCLUDE_DIR: ${OpenEXR_INCLUDE_DIR}")
message(STATUS "OpenImageIO_INCLUDE_DIR: ${OpenImageIO_INCLUDE_DIR}")
message(STATUS "PCRE_INCLUDE_DIR: ${PCRE_INCLUDE_DIR}")
message(STATUS "PDAL_INCLUDE_DIRS: ${PDAL_INCLUDE_DIRS}")
message(STATUS "proj_INCLUDE_DIR: ${proj_INCLUDE_DIR}")
message(STATUS "Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS}")
message(STATUS "PYTHON_NUMPY_INCLUDE_DIRS: ${PYTHON_NUMPY_INCLUDE_DIRS}")
message(STATUS "range-v3_INCLUDE_DIRS: ${range-v3_INCLUDE_DIRS}")
message(STATUS "Simd_INCLUDE_DIR: ${Simd_INCLUDE_DIR}")
message(STATUS "SLTBENCH_INCLUDE_DIRS: ${SLTBENCH_INCLUDE_DIRS}")
message(STATUS "TBB_INCLUDE_DIRS: ${Taskflow_INCLUDE_DIR}")
message(STATUS "TBB_INCLUDE_DIRS: ${TBB_INCLUDE_DIRS}")
message(STATUS "tiff_INCLUDE_DIR: ${tiff_INCLUDE_DIR}")
message(STATUS "tinyply_INCLUDE_DIR: ${tinyply_INCLUDE_DIR}")
message(STATUS "wxwidgets_INCLUDE_DIRS: ${wxwidgets_INCLUDE_DIRS}")
message(STATUS "vld_INCLUDE_DIR: ${vld_INCLUDE_DIR}")
message(STATUS "LIBXML2_INCLUDE_DIRS: ${LIBXML2_INCLUDE_DIRS}")
message(STATUS "xsimd_INCLUDE_DIRS: ${xsimd_INCLUDE_DIRS}")
message(STATUS "xtensor_INCLUDE_DIRS: ${xtensor_INCLUDE_DIRS}")
message(STATUS "xtensor-python_INCLUDE_DIRS: ${xtensor-python_INCLUDE_DIRS}")
message(STATUS "xtl_INCLUDE_DIRS: ${xtensor_INCLUDE_DIRS}")
message(STATUS "zlib_INCLUDE_DIR: ${zlib_INCLUDE_DIR}")

include_directories(${Boost_INCLUDE_DIRS}) 
include_directories(${CEREAL_INCLUDE_DIR})
include_directories(${CERES_INCLUDE_DIRS})
include_directories(${CGAL_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})
#include_directories(${EIGEN3BLAS_INCLUDE_DIR})
#include_directories(${EIGEN3LAPACK_INCLUDE_DIR})
include_directories(${exiv2_INCLUDE_DIR})
include_directories(${fmt_INCLUDE_DIRS})
include_directories(${GDAL_INCLUDE_DIRS})
include_directories(${Geogram_INCLUDE_DIR})
include_directories(${geos_INCLUDE_DIR})
include_directories(${geotiff_INCLUDE_DIR})
include_directories(${geotrans_INCLUDE_DIR})
include_directories(${GETTEXT_INCLUDE_DIR})
#include_directories(${GFLAGS_INCLUDE_DIR})
include_directories(${GLIB_INCLUDE_DIR})
include_directories(${GLOG_INCLUDE_DIR})
include_directories(${gsl_INCLUDE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GTS_INCLUDE_DIR})
include_directories(${ICONV_INCLUDE_DIRS})
include_directories(${libInterpolate_INCLUDE_DIRS})
include_directories(${lapack_INCLUDE_DIR})
include_directories(${LASLIB_INCLUDE_DIR})
include_directories(${magic_enum_INCLUDE_DIRECTORIES})
include_directories(${matplotlib_INCLUDE_DIR})
include_directories(${MPFR4_INCLUDE_DIR})
include_directories(${MPIR_INCLUDE_DIR})
include_directories(${nlohmann_json_INCLUDE_DIR})
include_directories(${nsimd_INCLUDE_DIR})
include_directories(${NumCpp_INCLUDE_DIR})
include_directories(${OpenBLAS_INCLUDE_DIR})
include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${OpenEXR_INCLUDE_DIR})
include_directories(${OpenImageIO_INCLUDE_DIR})
include_directories(${PCRE_INCLUDE_DIR})
include_directories(${PDAL_INCLUDE_DIRS})
include_directories(${proj_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${pybind11_INCLUDE_DIRS}) 
include_directories(${PYTHON_INCLUDE_DIRS}) 
include_directories(${PYTHON_NUMPY_INCLUDE_DIRS}) 
include_directories(${range-v3_INCLUDE_DIRS}) 
include_directories(${Simd_INCLUDE_DIR})
include_directories(${SLTBENCH_INCLUDE_DIRS})
include_directories(${Taskflow_INCLUDE_DIR})
include_directories(${TBB_INCLUDE_DIRS})
include_directories(${tiff_INCLUDE_DIR})
include_directories(${tinyply_INCLUDE_DIR})
include_directories(${wxwidgets_INCLUDE_DIRS})
include_directories(${vld_INCLUDE_DIR})
include_directories(${LIBXML2_INCLUDE_DIRS})
include_directories(${xsimd_INCLUDE_DIRS})
include_directories(${xtensor_INCLUDE_DIRS})
include_directories(${xtensor-python_INCLUDE_DIRS})
include_directories(${xtl_INCLUDE_DIRS})
include_directories(${zlib_INCLUDE_DIR})

include_directories("include")
include_directories("internal/include")

# section link
message(STATUS "PYTHON_ROOT: ${PYTHON_ROOT}")
message(STATUS "PYTHON_LIBRARY_DIRS: ${PYTHON_LIBRARY_DIRS}")

#link_directories(${Eigen3_LIBRARY_DIR}/${CDEGREL})
link_directories(${exiv2_LIBRARY_DIR}/${CDEGREL})
link_directories(${gsl_LIBRARY_DIR}/${CDEGREL})
link_directories(${OpenBLAS_LIBRARY_DIR}/${CDEGREL})
link_directories(${OpenCL_LIBRARY_DIR})
link_directories(${PYTHON_LIBRARY_DIRS})
link_directories(${zlib_LIBRARY_DIR})

# section source
add_subdirectory(java)
add_subdirectory(js)
add_subdirectory(python)
add_subdirectory(src)
enable_testing()
add_subdirectory(tests)

# section install
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  install(DIRECTORY ${GDAL_ROOT}/bin/debug/ 
          DESTINATION bin/debug
          CONFIGURATIONS Debug
          FILES_MATCHING PATTERN "*.dll" PATTERN "*.pdb")
  install(DIRECTORY ${GDAL_ROOT}/bin/release/ 
          DESTINATION bin/release
          CONFIGURATIONS Release
          FILES_MATCHING PATTERN "*.dll")
  #install(DIRECTORY ${HDF5_BIN_DIR}/
  #        DESTINATION bin 
  #        FILES_MATCHING PATTERN "*.dll")
  #install(DIRECTORY ${OpenCV_BIN_DIR}/
  #            DESTINATION bin 
  #        FILES_MATCHING PATTERN "*.dll" PATTERN "*.pdb")
  #install(DIRECTORY ${QT_BIN_DIR}/
  #        DESTINATION bin 
  #        FILES_MATCHING PATTERN "*.dll")

  set(GDAL_DATA "${GDAL_ROOT}/data/")
  set(OpenCV_DATA "${OpenCV_ROOT}/etc/")
else()
  # TODO
  set(GDAL_DATA "${GDAL_ROOT}/share/gdal/")
  set(OpenCV_DATA "${OpenCV_ROOT}/share/opencv4/")
  set(OpenCV_LICENSES "${OpenCV_ROOT}/share/licenses/opencv4/")
endif()

install(DIRECTORY include DESTINATION sdk)
install(DIRECTORY ${GDAL_DATA} DESTINATION etc/gdal)
install(DIRECTORY ${OpenCV_DATA} DESTINATION etc/opencv)

if(OpenCV_LICENSES)
  install(DIRECTORY ${OpenCV_LICENSES} DESTINATION etc/opencv/licenses)
endif()

install(DIRECTORY cmake DESTINATION sdk/lib)

option(EXAI_INSTALL_PACKAGES_3RDPARTY "install 3rdparty packages" ON)
if(EXAI_INSTALL_PACKAGES_3RDPARTY)
endif()

include(CPack)
