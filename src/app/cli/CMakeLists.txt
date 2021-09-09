project(cli CXX)

add_cli_executable(exai 
  SRCS "exai.cpp"
  ITEMS ex3rdparty core dataio epipolar epipolarmodel marshaller pfexai
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} glog::glog ${PDAL_LIBRARIES})
add_cli_executable(exdelaunay 
  SRCS "delaunay.cpp"
  ITEMS ex3rdparty core dataio exutils
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} glog::glog)
add_cli_executable(exdensematch 
  SRCS "densematch.cpp"
  ITEMS ex3rdparty core dataio epipolarmodel marshaller pfexai sfm
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} glog::glog)
add_cli_executable(exepipolar 
  SRCS "epipolar.cpp"
  ITEMS ex3rdparty core dataio epipolar epipolarmodel marshaller pfexai
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} glog::glog)
add_cli_executable(exgdal 
  SRCS "gdal.cpp"
  ITEMS ex3rdparty core dataio pfgdal
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} glog::glog)
add_cli_executable(exgeoinfo 
  SRCS "geoinfo.cpp"
  ITEMS ex3rdparty core dataio
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} glog::glog)
add_cli_executable(exortho 
  SRCS "ortho.cpp"
  ITEMS ex3rdparty core dataio marshaller ortho pfexai sensormodel
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} glog::glog)
add_cli_executable(expdal 
  SRCS "pdal.cpp"
  ITEMS ex3rdparty core dataio pfpdal
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} glog::glog ${PDAL_LIBRARIES})
add_cli_executable(exsparsematch 
  SRCS "sparsematch.cpp"
  ITEMS ex3rdparty core dataio epipolarmodel marshaller pfexai sfm
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} glog::glog)

exai_install_target(exai)
exai_install_target(exdelaunay)
exai_install_target(exdensematch)
exai_install_target(exepipolar)
exai_install_target(exgdal)
exai_install_target(exgeoinfo)
exai_install_target(exortho)
exai_install_target(expdal)
exai_install_target(exsparsematch)
