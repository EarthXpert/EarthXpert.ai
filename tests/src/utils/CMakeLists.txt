project(testutils CXX)

add_definitions(-DEXTESTUTILS_API)
set(testutils_srcs 
  "./testutils.cpp")
set(testutils_items ex3rdparty blockadjust core dataio epipolar epipolarmodel exutils 
  imageproc marshaller pcproc sensormodel sfm
  Boost::boost Boost::filesystem Boost::iostreams
  ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES}
  ${LASLIB_LIBRARY} ${OpenCV_LIBS})
exai_add_library(testutils "tests"
  SRCS ${testutils_srcs}
  ITEMS ${testutils_items})

exai_source_group(${testutils_srcs})
exai_install_target(testutils)
