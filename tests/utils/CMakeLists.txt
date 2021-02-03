project(testutils CXX)

add_definitions(-DEXTESTUTILS_API)
set(testutils_srcs "./testutils.cpp")
set(testutils_items ex3rdparty blockadjust core dataio epipolar epipolarmodel exutils 
  imageproc marshaller pcproc sensormodel sfm
  Boost::boost Boost::filesystem Boost::iostreams
  ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARY} ${geotrans_LIBRARIES}
  ${GETTEXT_LIBRARY} ${LASLIB_LIBRARY} ${OpenCV_LIBS})
earthxpert_add_library(testutils "tests"
  SRCS ${testutils_srcs}
  ITEMS ${testutils_items})

earthxpert_source_group(${testutils_srcs})
earthxpert_install_target(testutils)
