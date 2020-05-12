project(wx3rd_party)

add_definitions(-DWX3RDPARTY_API)
watxtract_add_library(wx3rd_party "api"
  SRCS "gdal/wxgdal.cpp"
       "gtest/wxgtest.cpp"
       "laslib/wxlaslib.cpp"
       "opencv/wxopencv.cpp"
       "sltbench/wxsltbench.cpp"
  ITEMS ${Boost_LIBRARIES} ${GDAL_LIBRARY} ${GTEST_BOTH_LIBRARIES} 
        ${LASLIB_LIBRARY} ${OPENCV_LIBS} ${SLTBENCH_LIBS})

watxtract_install_target(wx3rd_party)
