project(cli CXX)

add_cli_executable(exai 
  SRCS "exai.cpp"
  ITEMS ex3rdparty core dataio epipolar epipolarmodel marshaller pfexai
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} ${PDAL_LIBRARIES})
add_cli_executable(exepipolar 
  SRCS "epipolar.cpp"
  ITEMS ex3rdparty core dataio epipolar epipolarmodel marshaller pfexai
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY} ${geotrans_LIBRARIES})
add_cli_executable(exgdal 
  SRCS "gdal.cpp"
  ITEMS ex3rdparty core dataio pfgdal
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY})
add_cli_executable(exgeoinfo 
  SRCS "geoinfo.cpp"
  ITEMS ex3rdparty core dataio
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY})
add_cli_executable(expdal 
  SRCS "pdal.cpp"
  ITEMS ex3rdparty core dataio pfpdal
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${PDAL_LIBRARIES})

earthxpert_install_target(exai)
earthxpert_install_target(exepipolar)
earthxpert_install_target(exgdal)
earthxpert_install_target(exgeoinfo)
earthxpert_install_target(expdal)
