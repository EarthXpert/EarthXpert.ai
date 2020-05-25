project(ex3rd_party)

add_definitions(-DEX3RDPARTY_API)
earthxpert_add_library(ex3rd_party "api"
  SRCS "boost/exboost.cpp"
       "ceres/exceres.cpp"
       "eigen/exeigen.cpp"
       "gdal/exgdal.cpp"
       "gtest/exgtest.cpp"
       "laslib/exlaslib.cpp"
       "opencv/exopencv.cpp"
       "sltbench/exsltbench.cpp"
  ITEMS ${Boost_LIBRARIES} ${CERES_LIBRARIES} Eigen3::Eigen ${GDAL_LIBRARY} 
        ${GTEST_BOTH_LIBRARIES} ${LASLIB_LIBRARY} ${OPENCV_LIBS} ${SLTBENCH_LIBS})

earthxpert_install_target(ex3rd_party)
