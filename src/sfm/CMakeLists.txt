project(sfm CXX)

add_definitions(-DEXSFM_API)
set(sfm_srcs 
  "densematch/circleneighbor.cpp"
  "densematch/densematch.cpp"
  "densematch/densematchfactory.cpp"
  "densematch/depthmatch.cpp"
  "densematch/disparitymatch.cpp"
  "densematch/rectneighbor.cpp"
  "sparsematch/featdetectperf.cpp"
  "sparsematch/featuredetect.cpp"
  "sparsematch/featuredetect_layout.cpp"
  "sparsematch/sparsematch.cpp")
set(sfm_items ex3rdparty core dataio epipolarmodel frameworks sensormodel
  ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${OpenCV_LIBS})
exai_add_library(sfm "api"
  SRCS ${sfm_srcs}
  ITEMS ${sfm_items})

exai_source_group(${sfm_srcs})
exai_install_target(sfm)
