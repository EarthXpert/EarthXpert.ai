project(sfm)

add_definitions(-DWXSFM_API)
watxtract_add_library(sfm "api"
  SRCS "densematch/circleneighbor.cpp"
       "densematch/densematch.cpp"
       "densematch/densematchfactory.cpp"
       "densematch/depthmatch.cpp"
       "densematch/disparitymatch.cpp"
       "densematch/rectneighbor.cpp"
       "sparsematch/sparsematch.cpp"
  ITEMS wx3rd_party core dataio epipolarmodel sensormodel
        ${GDAL_LIBRARY} ${OPENCV_LIBS})

watxtract_install_target(sfm)
