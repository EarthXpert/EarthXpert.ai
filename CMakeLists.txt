# top-level CMake configuration file for EarthXpert.ai
cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
if(POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

project(earthxpert)
set(EARTHXPERT_VERSION 1.0.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/sdk/lib)
set(EARTHXPERT_WORKSPACE_DIR "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/../>")
message(STATUS "EARTHXPERT_WORKSPACE_DIR: ${EARTHXPERT_WORKSPACE_DIR}")

set(EX_THIRD_PARTY "$ENV{EX_Third_Party}")
set(CUDA_ROOT "$ENV{CUDA_PATH}")
set(OpenCL_ROOT "$ENV{CUDA_PATH}")

#set(GFLAGS_USE_TARGET_NAMESPACE TRUE)

# configuration & utils
add_subdirectory(cmake)

# section third-party
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if (MSVC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W1")
  endif()
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DCPL_DISABLE_DLL)

  set(EARTHXPERT_DEBUG_POSTFIX d)
  set(CDEGREL $<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>)
  message(STATUS "Debug_Release_Mode: ${CDEGREL}")

  set(EARTHXPERT_BIN_ROOT "bin")
  set(EARTHXPERT_BIN_DIR "${EARTHXPERT_BIN_ROOT}/${CDEGREL}")

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

  set(Eigen3_DIR "${EX_THIRD_PARTY}/eigen_3.3.90/share/eigen3/cmake")

  set(GDAL_ROOT "${EX_THIRD_PARTY}/gdal_2.4.1")
  set(GDAL_BIN_DIR "${GDAL_ROOT}/bin/${CDEGREL}")
  set(GDAL_INCLUDE_DIR "${GDAL_ROOT}/include")
  set(GDAL_LIBRARY "${GDAL_ROOT}/lib/${CDEGREL}/gdal_i.lib")

  set(geotiff_ROOT "${EX_THIRD_PARTY}/libgeotiff_1.4.2")
  set(geotiff_BIN_DIR "${geotiff_ROOT}/bin/${CDEGREL}")
  set(geotiff_DIR "${geotiff_ROOT}/share/libgeotiff")
  set(geotiff_INCLUDE_DIR "${geotiff_ROOT}/include")
  set(geotiff_LIBRARY_DIR "${geotiff_ROOT}/lib")
  set(geotiff_LIBRARIES debug "${geotiff_LIBRARY_DIR}/debug/geotiff_d_i.lib"
                        optimized "${geotiff_LIBRARY_DIR}/release/geotiff_i.lib")

  set(geotrans_ROOT "${EX_THIRD_PARTY}/geotrans_3.8.0")
  set(geotrans_BIN_DIR "${geotrans_ROOT}/bin/${CDEGREL}")
  set(geotrans_INCLUDE_DIR "${geotrans_ROOT}/include")
  set(geotrans_LIBRARIES "${geotrans_ROOT}/lib/${CDEGREL}/MSPCoordinateConversionService.lib"
                         "${geotrans_ROOT}/lib/${CDEGREL}/MSPdtcc.lib")

  set(GETTEXT_ROOT "${EX_THIRD_PARTY}/gettext_0.19.0")
  set(GETTEXT_BIN_DIR "${GETTEXT_ROOT}/bin/${CDEGREL}")
  set(GETTEXT_INCLUDE_DIR "${GETTEXT_ROOT}/include")
  set(GETTEXT_LIBRARY "${GETTEXT_ROOT}/lib/${CDEGREL}/libintl.lib")

  #set(GFLAGS_ROOT "${EX_THIRD_PARTY}/gflags_2.2.2")
  #set(gflags_DIR "${GFLAGS_ROOT}/lib/cmake/gflags")
  #set(GFLAGS_INCLUDE_DIR "${GFLAGS_ROOT}/include")

  set(GLOG_ROOT "${EX_THIRD_PARTY}/glog_0.4.0")
  set(glog_DIR "${GLOG_ROOT}/lib/cmake/glog")
  set(GLOG_INCLUDE_DIR "${GLOG_ROOT}/include")

  set(gsl_ROOT "${EX_THIRD_PARTY}/gsl_2.4.0")
  set(gsl_BIN_DIR "${gsl_ROOT}/bin/${CDEGREL}")
  set(gsl_INCLUDE_DIR "${gsl_ROOT}/include")
  set(gsl_LIBRARY_DIR "${gsl_ROOT}/lib")
  set(gsl_LIBRARIES debug "${gsl_LIBRARY_DIR}/debug/gslcblasd.lib"
                    debug "${gsl_LIBRARY_DIR}/debug/gsld.lib"
                    optimized "${gsl_LIBRARY_DIR}/release/gslcblas.lib"
                    optimized "${gsl_LIBRARY_DIR}/release/gsl.lib")

  set(GTEST_ROOT "${EX_THIRD_PARTY}/gtest_1.8.1")
  set(GTEST_BIN_DIR "${GTEST_ROOT}/bin")
  set(GTEST_INCLUDE_DIR "${GTEST_ROOT}/include")
  
  set(HDF5_ROOT "${EX_THIRD_PARTY}/hdf5_1.10.5")
  set(HDF5_BIN_DIR "${HDF5_ROOT}/bin")
  
  set(LASLIB_ROOT "${EX_THIRD_PARTY}/lastools_19.05.07")
  #set(LASLIB_BIN_DIR "${LASLIB_ROOT}/bin")
  set(LASLIB_INCLUDE_DIR "${LASLIB_ROOT}/include/LASlib")
  set(LASLIB_LIBRARY "${LASLIB_ROOT}/lib/${CDEGREL}/LASlib/LASlib.lib")

  set(libInterpolate_DIR "${EX_THIRD_PARTY}/libInterpolate_2.3.2/cmake")

  set(LIBXML2_ROOT "${EX_THIRD_PARTY}/xml2_2.9.9")
  set(LIBXML2_DIR "${LIBXML2_ROOT}/share/libxml2/CMake")
  set(LIBXML2_INCLUDE_DIR "${LIBXML2_ROOT}/include")
  set(LIBXML2_LIBRARY_DIR "${LIBXML2_ROOT}/lib")
  set(LIBXML2_LIBRARY "${LIBXML2_LIBRARY_DIR}/${CDEGREL}/libxml2.lib")

  set(MPFR4_ROOT "${EX_THIRD_PARTY}/mpfr_4.0.2")
  #set(MPFR4_DIR "${MPFR4_ROOT}/share/mpfr")
  set(MPFR4_INCLUDE_DIR "${MPFR4_ROOT}/include")
  set(MPFR4_LIBRARY_DIR "${MPFR4_ROOT}/lib")
  set(MPFR4_LIBRARIES "${MPFR4_LIBRARY_DIR}/${CDEGREL}/mpfr.lib")

  set(MPIR_ROOT "${EX_THIRD_PARTY}/mpir_3.0.0")
  #set(MPIR_DIR "${MPIR_ROOT}/share/mpir")
  set(MPIR_INCLUDE_DIR "${MPIR_ROOT}/include")
  set(MPIR_LIBRARY_DIR "${MPIR_ROOT}/lib")
  set(MPIR_LIBRARIES "${MPIR_LIBRARY_DIR}/${CDEGREL}/mpir.lib")

  set(nlohmann_json_ROOT "${EX_THIRD_PARTY}/nlohmann-json_3.7.3")
  set(nlohmann_json_DIR "${nlohmann_json_ROOT}/share/nlohmann_json")
  set(nlohmann_json_INCLUDE_DIR "${nlohmann_json_ROOT}/include")

  set(nsimd_ROOT "${EX_THIRD_PARTY}/nsimd_20.11.01")
  #set(nsimd_DIR "${nsimd_ROOT}/lib/cmake")
  set(nsimd_BIN_DIR "${nsimd_ROOT}/bin/${CDEGREL}")
  set(nsimd_INCLUDE_DIR "${nsimd_ROOT}/include")
  set(nsimd_LIBRARY_DIR "${nsimd_ROOT}/lib")
  set(nsimd_LIBRARIES "${nsimd_LIBRARY_DIR}/${CDEGREL}/libnsimd_x86_64.lib")

  set(OpenCL_INCLUDE_DIRS "${OpenCL_ROOT}/include")
  set(OpenCL_BIN_DIR "${OpenCL_ROOT}/bin")
  set(OpenCL_LIBRARIES_DIR "${OpenCL_ROOT}/lib/x64")
  set(OpenCL_LIBRARIES "${OpenCL_LIBRARIES_DIR}/OpenCL.lib")

  set(OpenCV_ROOT "${EX_THIRD_PARTY}/opencv_4.1.0")
  set(OpenCV_DIR "${OpenCV_ROOT}")
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

  set(PDAL_ROOT "${EX_THIRD_PARTY}/pdal_1.9.1")
  set(PDAL_DIR "${PDAL_ROOT}/lib/cmake/PDAL")

  set(proj_ROOT "${EX_THIRD_PARTY}/proj_8.0.0")
  #set(proj_DIR "${proj_ROOT}/lib/cmake/proj")
  set(proj_BIN_DIR "${proj_ROOT}/bin/${CDEGREL}")
  set(proj_INCLUDE_DIR "${proj_ROOT}/include")
  set(proj_LIBRARY_DIR "${proj_ROOT}/lib")
  set(proj_LIBRARIES debug "${proj_LIBRARY_DIR}/debug/proj_d.lib"
                     optimized "${proj_LIBRARY_DIR}/release/proj.lib")

  set(Protobuf_ROOT "${EX_THIRD_PARTY}/protobuf_3.11.3")
  set(Protobuf_DIR "${Protobuf_ROOT}/share/protobuf")

  set(pybind11_DIR "${EX_THIRD_PARTY}/pybind11_2.5.0/share/cmake/pybind11")

  set(Qt5_DIR "${EX_THIRD_PARTY}/qt_5.11.2/lib/cmake/Qt5")

  set(Simd_ROOT "${EX_THIRD_PARTY}/Simd_4.6.94")
  #set(Simd_DIR "${Simd_ROOT}/lib/cmake")
  set(Simd_BIN_DIR "${Simd_ROOT}/bin/${CDEGREL}")
  set(Simd_INCLUDE_DIR "${Simd_ROOT}/include")
  set(Simd_LIBRARY_DIR "${Simd_ROOT}/lib")
  set(Simd_LIBRARIES "${Simd_LIBRARY_DIR}/${CDEGREL}/Simd.lib")

  set(Sltbench_DIR "${EX_THIRD_PARTY}/sltbench_20.01.15/cmake")

  set(sqlite3_ROOT "${EX_THIRD_PARTY}/sqlite_3.31.1")
  set(sqlite3_DIR "${EX_THIRD_PARTY}/share/sqlite3")

  set(TBB_ROOT "${EX_THIRD_PARTY}/tbb_20.0.0")
  set(TBB_DIR "${TBB_ROOT}/share/tbb")
  set(TBB_INCLUDE_DIRS "${TBB_ROOT}/include")
  set(TBB_LIBRARY_DIR "${TBB_ROOT}/lib")
  set(TBB_LIBRARIES debug "${TBB_LIBRARY_DIR}/debug/tbb_debug.lib"
                    debug "${TBB_LIBRARY_DIR}/debug/tbbmalloc_debug.lib"
                    debug "${TBB_LIBRARY_DIR}/debug/tbbmalloc_proxy_debug.lib"
                    optimized "${TBB_LIBRARY_DIR}/release/tbb.lib"
                    optimized "${TBB_LIBRARY_DIR}/release/tbbmalloc.lib"
                    optimized "${TBB_LIBRARY_DIR}/release/tbbmalloc_proxy.lib")

  set(tiff_ROOT "${EX_THIRD_PARTY}/tiff_4.0.10")
  set(tiff_BIN_DIR "${tiff_ROOT}/bin/${CDEGREL}")
  #set(tiff_DIR "${tiff_ROOT}/share/tiff")
  set(tiff_INCLUDE_DIR "${tiff_ROOT}/include")
  set(tiff_LIBRARY_DIR "${tiff_ROOT}/lib")
  set(tiff_LIBRARIES debug "${tiff_LIBRARY_DIR}/debug/tiffd.lib"
                     debug "${tiff_LIBRARY_DIR}/debug/tiffxxd.lib"
                     optimized "${tiff_LIBRARY_DIR}/release/tiff.lib"
                     optimized "${tiff_LIBRARY_DIR}/release/tiffxx.lib")

  set(xsimd_DIR "${EX_THIRD_PARTY}/xsimd_7.4.6/share/xsimd")

  #set(Python_ROOT_DIR "python37")

  set(vld_ROOT "${EX_THIRD_PARTY}/vld_2.5.1")
  #set(vld_DIR "${vld_ROOT}/lib/cmake")
  set(vld_BIN_DIR "${vld_ROOT}/bin")
  set(vld_INCLUDE_DIR "${vld_ROOT}/include")
  set(vld_LIBRARY_DIR "${vld_ROOT}/lib")
  set(vld_LIBRARIES debug "${vld_LIBRARY_DIR}/vld.lib")
  
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(EARTHXPERT_BIN_ROOT "bin")
  set(EARTHXPERT_BIN_DIR "bin")
  set(EARTHXPERT_DEBUG_POSTFIX "")

  set(CEREAL_INCLUDE_DIR "${EX_THIRD_PARTY}/cereal_1.2.2/include")

  set(GDAL_ROOT "${EX_THIRD_PARTY}/gdal_2.3.2")
  set(GDAL_INCLUDE_DIR "${GDAL_ROOT}/include")
  set(GDAL_LIBRARY "${GDAL_ROOT}/lib/libgdal.so")

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

message(STATUS "EARTHXPERT_BIN_DIR: ${EARTHXPERT_BIN_DIR}")

if(DEFINED CMAKE_DEBUG_POSTFIX)
  set(EARTHXPERT_DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
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
find_package(Boost 1.71.0 COMPONENTS filesystem iostreams locale math program_options python37 system) 

find_package(Ceres REQUIRED)
find_package(CGAL REQUIRED)
#find_package(CUDA REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(GDAL REQUIRED)
#find_package(geotrans REQUIRED)
#find_package(geotiff REQUIRED)
#find_package(GETTEXT REQUIRED)
#find_package(gflags REQUIRED)
find_package(glog REQUIRED)
#find_package(gsl REQUIRED)
find_package(GTest REQUIRED)
find_package(libInterpolate REQUIRED)
find_package(LIBXML2 REQUIRED)
#find_package(MPFR REQUIRED)
#find_package(MPIR REQUIRED)
find_package(nlohmann_json REQUIRED)
#find_package(nsimd REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(OpenCL REQUIRED)
#find_package(OpenEXR REQUIRED)
find_package(OpenImageIO REQUIRED)
find_package(PDAL REQUIRED)
#find_package(proj REQUIRED)
find_package(Protobuf REQUIRED)
find_package(pybind11 REQUIRED)
find_package(Qt5 5.11.2 COMPONENTS Core Gui LinguistTools OpenGL Widgets REQUIRED)
#find_package(Simd REQUIRED)
find_package(sqlite3 REQUIRED)
find_package(Sltbench REQUIRED)
#find_package(TBB REQUIRED)
#find_package(tiff REQUIRED)
find_package(xsimd REQUIRED)
#find_package(vld)

find_package(Java 1.8 REQUIRED Runtime Development)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

#earthxpert_get_INCLUDE_DIRS(libInterpolate libInterpolate::Interpolate)
get_target_property(libInterpolate_INCLUDE_DIRS libInterpolate::Interpolate INTERFACE_INCLUDE_DIRECTORIES)

#earthxpert_get_INCLUDE_DIRS(SLTBENCH sltbench)
get_target_property(SLTBENCH_INCLUDE_DIRS sltbench INTERFACE_INCLUDE_DIRECTORIES)

#earthxpert_get_LIBRARIES(SLTBENCH sltbench)
get_target_property(SLTBENCH_LIBRARY_DEBUG sltbench IMPORTED_LOCATION_DEBUG)
get_target_property(SLTBENCH_LIBRARY_RELEASE sltbench IMPORTED_LOCATION_RELEASE)
set(SLTBENCH_LIBRARIES debug ${SLTBENCH_LIBRARY_DEBUG}
                       optimized ${SLTBENCH_LIBRARY_RELEASE})

# remark: BOOST_BIN_DIR is empty in subdirectory!
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(BOOST_BIN_DIR "${Boost_DIR}/../..")
  message(STATUS "BOOST_BIN_DIR: ${BOOST_BIN_DIR}")
else()

endif()

# section include
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "CEREAL_INCLUDE_DIR: ${CEREAL_INCLUDE_DIR}")
message(STATUS "CERES_INCLUDE_DIRS: ${CERES_INCLUDE_DIRS}")
message(STATUS "CGAL_INCLUDE_DIRS: ${CGAL_INCLUDE_DIRS}")
message(STATUS "CUDA_INCLUDE_DIR: ${CUDA_INCLUDE_DIR}")
message(STATUS "EIGEN3_INCLUDE_DIR: ${EIGEN3_INCLUDE_DIR}")
message(STATUS "GDAL_INCLUDE_DIR: ${GDAL_INCLUDE_DIR}")
message(STATUS "geotiff_INCLUDE_DIR: ${geotiff_INCLUDE_DIR}")
message(STATUS "geotrans_INCLUDE_DIR: ${geotrans_INCLUDE_DIR}")
message(STATUS "GETTEXT_INCLUDE_DIR: ${GDAL_INCLUDE_DIR}")
#message(STATUS "GFLAGS_INCLUDE_DIR: ${GLOG_INCLUDE_DIR}")
message(STATUS "GLOG_INCLUDE_DIR: ${GLOG_INCLUDE_DIR}")
message(STATUS "gsl_INCLUDE_DIR: ${gsl_INCLUDE_DIR}")
message(STATUS "GTEST_INCLUDE_DIRS: ${GTEST_INCLUDE_DIRS}")
message(STATUS "LASLIB_INCLUDE_DIR: ${LASLIB_INCLUDE_DIR}")
message(STATUS "libInterpolate_INCLUDE_DIRS: ${libInterpolate_INCLUDE_DIRS}")
message(STATUS "LIBXML2_INCLUDE_DIR: ${LIBXML2_INCLUDE_DIR}")
message(STATUS "MPFR_INCLUDE_DIR: ${MPFR_INCLUDE_DIR}")
message(STATUS "MPFR4_INCLUDE_DIR: ${MPFR4_INCLUDE_DIR}")
message(STATUS "MPIR_INCLUDE_DIR: ${MPIR_INCLUDE_DIR}")
message(STATUS "nlohmann_json_INCLUDE_DIR: ${nlohmann_json_INCLUDE_DIR}")
message(STATUS "nsimd_INCLUDE_DIR: ${nsimd_INCLUDE_DIR}")
message(STATUS "OpenCL_INCLUDE_DIRS: ${OpenCL_INCLUDE_DIRS}")
message(STATUS "OpenCV_INCLUDE_DIR: ${OpenCV_INCLUDE_DIR}")
message(STATUS "OpenEXR_INCLUDE_DIR: ${OpenEXR_INCLUDE_DIR}")
message(STATUS "OpenImageIO_INCLUDE_DIR: ${OpenImageIO_INCLUDE_DIR}")
message(STATUS "PDAL_INCLUDE_DIRS: ${PDAL_INCLUDE_DIRS}")
message(STATUS "proj_INCLUDE_DIR: ${proj_INCLUDE_DIR}")
message(STATUS "Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "Simd_INCLUDE_DIR: ${Simd_INCLUDE_DIR}")
message(STATUS "SLTBENCH_INCLUDE_DIRS: ${SLTBENCH_INCLUDE_DIRS}")
message(STATUS "TBB_INCLUDE_DIRS: ${TBB_INCLUDE_DIRS}")
message(STATUS "tiff_INCLUDE_DIR: ${tiff_INCLUDE_DIR}")
message(STATUS "xsimd_INCLUDE_DIRS: ${xsimd_INCLUDE_DIRS}")
message(STATUS "vld_INCLUDE_DIR: ${vld_INCLUDE_DIR}")

include_directories(${Boost_INCLUDE_DIRS}) 
include_directories(${CEREAL_INCLUDE_DIR})
include_directories(${CERES_INCLUDE_DIRS})
include_directories(${CGAL_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${GDAL_INCLUDE_DIR})
include_directories(${geotiff_INCLUDE_DIR})
include_directories(${geotrans_INCLUDE_DIR})
include_directories(${GETTEXT_INCLUDE_DIR})
#include_directories(${GFLAGS_INCLUDE_DIR})
include_directories(${GLOG_INCLUDE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${gsl_INCLUDE_DIR})
include_directories(${LASLIB_INCLUDE_DIR})
include_directories(${LIBXML2_INCLUDE_DIR})
include_directories(${libInterpolate_INCLUDE_DIRS})
include_directories(${MPFR4_INCLUDE_DIR})
include_directories(${MPIR_INCLUDE_DIR})
include_directories(${nlohmann_json_INCLUDE_DIR})
include_directories(${nsimd_INCLUDE_DIR})
include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${OpenEXR_INCLUDE_DIR})
include_directories(${OpenImageIO_INCLUDE_DIR})
include_directories(${PDAL_INCLUDE_DIRS})
include_directories(${proj_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${Simd_INCLUDE_DIR})
include_directories(${SLTBENCH_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIRS})
include_directories(${tiff_INCLUDE_DIR})
include_directories(${xsimd_INCLUDE_DIRS})
include_directories(${vld_INCLUDE_DIR})

include_directories("include")

# section link
link_directories(${OpenCL_LIBRARY_DIR})
link_directories(${gsl_LIBRARY_DIR})

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

option(EARTHXPERT_INSTALL_PACKAGES_3RDPARTY "install 3rdparty packages" OFF)
if(EARTHXPERT_INSTALL_PACKAGES_3RDPARTY)
endif()

include(CPack)
