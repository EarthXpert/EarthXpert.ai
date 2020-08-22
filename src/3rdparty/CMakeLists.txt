project(ex3rdparty)

message(STATUS "CERES_LIBRARY: ${CERES_LIBRARY}")
message(STATUS "CGAL_LIBRARIES: ${CGAL_LIBRARIES}")
message(STATUS "GDAL_LIBRARY: ${GDAL_LIBRARY}")
message(STATUS "GETTEXT_LIBRARY: ${GETTEXT_LIBRARY}")
message(STATUS "GTEST_BOTH_LIBRARIES: ${GTEST_BOTH_LIBRARIES}")
message(STATUS "LASLIB_LIBRARY: ${LASLIB_LIBRARY}")
message(STATUS "LIBXML2_LIBRARIES: ${LIBXML2_LIBRARIES}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
message(STATUS "PDAL_LIBRARIES: ${PDAL_LIBRARIES}")
message(STATUS "Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")
message(STATUS "SLTBENCH_LIBS: ${SLTBENCH_LIBS}")

add_definitions(-DEX3RDPARTY_API)
set(ex3rdparty_srcs "boost/exboost.cpp"
       "ceres/bal_problem.cpp"
       "ceres/exceres.cpp"
       "cgal/excgal.cpp"
       "cuda/excuda.cpp"
       "eigen/exeigen.cpp"
       "gdal/exgdal.cpp"
       "gettext/exgettext.cpp"
       "gflags/exgflags.cpp"
       "glog/exglog.cpp"
       "gtest/exgtest.cpp"
       "laslib/exlaslib.cpp"
       "opencl/exopencl.cpp"
       "opencv/exopencv.cpp"
       "pdal/expdal.cpp"
       "protobuf/exprotobuf.cpp"
       "pybind11/expybind11.cpp"
       "qt/exqt.cpp"
       "sltbench/exsltbench.cpp"
       "thrust/exthrust.cpp"
       "wxwidgets/exwxwidgets.cpp"
       "xml/exxml.cpp")
earthxpert_add_library(ex3rdparty "api"
  SRCS ${ex3rdparty_srcs}
  ITEMS Boost::boost Boost::filesystem Boost::locale ${CERES_LIBRARY} ${CGAL_LIBRARIES} 
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} glog::glog ${GTEST_BOTH_LIBRARIES}
         ${LASLIB_LIBRARY} ${LIBXML2_LIBRARIES} ${OpenCV_LIBS} ${OpenEXR_LIBS} 
        OpenImageIO::OpenImageIO OpenImageIO::OpenImageIO_Util 
        ${PDAL_LIBRARIES} ${Protobuf_LIBRARIES} Qt5::Widgets ${SLTBENCH_LIBS})

earthxpert_source_group(${ex3rdparty_srcs})
earthxpert_install_target(ex3rdparty)
