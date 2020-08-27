project(sfm CXX)

add_definitions(-DEXSFM_API)
set(sfm_srcs "densematch/circleneighbor.cpp"
  "densematch/densematch.cpp"
  "densematch/densematchfactory.cpp"
  "densematch/depthmatch.cpp"
  "densematch/disparitymatch.cpp"
  "densematch/rectneighbor.cpp"
  "sparsematch/featdetectperf.cpp"
  "sparsematch/featuredetect.cpp"
  "sparsematch/featuredetect_layout.cpp"
  "sparsematch/sparsematch.cpp")
earthxpert_add_library(sfm "api"
  SRCS ${sfm_srcs}
  ITEMS ex3rdparty core dataio epipolarmodel sensormodel
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCV_LIBS})

earthxpert_source_group(${sfm_srcs})
earthxpert_install_target(sfm)
