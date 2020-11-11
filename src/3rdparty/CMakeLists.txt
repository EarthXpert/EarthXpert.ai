project(ex3rdparty)

message(STATUS "CERES_LIBRARY: ${CERES_LIBRARY}")
message(STATUS "CGAL_LIBRARIES: ${CGAL_LIBRARIES}")
message(STATUS "GDAL_LIBRARY: ${GDAL_LIBRARY}")
message(STATUS "GETTEXT_LIBRARY: ${GETTEXT_LIBRARY}")
message(STATUS "GTEST_BOTH_LIBRARIES: ${GTEST_BOTH_LIBRARIES}")
message(STATUS "LASLIB_LIBRARY: ${LASLIB_LIBRARY}")
message(STATUS "LIBXML2_LIBRARIES: ${LIBXML2_LIBRARIES}")
message(STATUS "nsimd_LIBRARIES: ${nsimd_LIBRARIES}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
message(STATUS "PDAL_LIBRARIES: ${PDAL_LIBRARIES}")
message(STATUS "Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")
message(STATUS "Simd_LIBRARIES: ${Simd_LIBRARIES}")
message(STATUS "SLTBENCH_LIBS: ${SLTBENCH_LIBS}")

add_definitions(-DEX3RDPARTY_API)
set(ex3rdparty_srcs "boost/exboost.cpp"
  "ceres/bal_problem.cpp"
  "ceres/exceres.cpp"
  "cgal/excgal.cpp"
  "cuda/excuda.cpp"
  "eigen/exeigen.cpp"
  "gdal/exgdal.cpp"
  "geotrans/exgeotrans.cpp"
  "gettext/exgettext.cpp"
  "gflags/exgflags.cpp"
  "glog/exglog.cpp"
  "gtest/exgtest.cpp"
  "laslib/exlaslib.cpp"
  "nsimd/exnsimd.cpp"
  "opencl/exopencl.cpp"
  "opencv/exopencv.cpp"
  "pdal/expdal.cpp"
  "protobuf/exprotobuf.cpp"
  "pybind11/expybind11.cpp"
  "qt/exqt.cpp"
  "simd/exsimd.cpp"
  "sltbench/exsltbench.cpp"
  "thrust/exthrust.cpp"
  "stl/exstl.cpp"
  "vld/exvld.cpp"
  "vxl/exvxl.cpp"
  "wxwidgets/exwxwidgets.cpp"
  "xml/exxml.cpp"
  "xsimd/exxsimd.cpp")
earthxpert_add_library(ex3rdparty "api"
  SRCS ${ex3rdparty_srcs}
  ITEMS Boost::boost Boost::filesystem Boost::locale ${CERES_LIBRARY} ${CGAL_LIBRARIES} 
        ${GDAL_LIBRARY} ${geotrans_LIBRARY} ${GETTEXT_LIBRARY} glog::glog ${GTEST_BOTH_LIBRARIES}
         ${LASLIB_LIBRARY} ${LIBXML2_LIBRARIES} ${OpenCV_LIBS} ${OpenEXR_LIBS} 
        OpenImageIO::OpenImageIO OpenImageIO::OpenImageIO_Util 
        ${nsimd_LIBRARIES} ${PDAL_LIBRARIES} ${Protobuf_LIBRARIES} Qt5::Widgets 
        ${Simd_LIBRARIES} ${SLTBENCH_LIBS})

earthxpert_source_group(${ex3rdparty_srcs})
earthxpert_install_target(ex3rdparty)
