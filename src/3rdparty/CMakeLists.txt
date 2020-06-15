project(ex3rdparty)

message(STATUS "CERES_LIBRARY: ${CERES_LIBRARY}")
message(STATUS "CGAL_LIBRARIES: ${CGAL_LIBRARIES}")
message(STATUS "GDAL_LIBRARY: ${GDAL_LIBRARY}")
message(STATUS "GETTEXT_LIBRARY: ${GETTEXT_LIBRARY}")
message(STATUS "GTEST_BOTH_LIBRARIES: ${GTEST_BOTH_LIBRARIES}")
message(STATUS "LASLIB_LIBRARY: ${LASLIB_LIBRARY}")
message(STATUS "OPENCV_LIBS: ${OPENCV_LIBS}")
message(STATUS "SLTBENCH_LIBS: ${SLTBENCH_LIBS}")

add_definitions(-DEX3RDPARTY_API)
earthxpert_add_library(ex3rdparty "api"
  SRCS "boost/exboost.cpp"
       "ceres/exceres.cpp"
       "cgal/excgal.cpp"
       "eigen/exeigen.cpp"
       "gdal/exgdal.cpp"
       "gettext/exgettext.cpp"
       "gtest/exgtest.cpp"
       "laslib/exlaslib.cpp"
       "opencv/exopencv.cpp"
       "pdal/expdal.cpp"
       "qt/exqt.cpp"
       "sltbench/exsltbench.cpp"
  ITEMS Boost::boost Boost::filesystem ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} 
        ${GETTEXT_LIBRARY} glog::glog ${GTEST_BOTH_LIBRARIES} ${LASLIB_LIBRARY} ${OPENCV_LIBS} 
        ${PDAL_LIBRARIES} Qt5::Widgets ${SLTBENCH_LIBS})

earthxpert_install_target(ex3rdparty)
