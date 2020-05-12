project(tools)

watxtract_add_executable(wxepipolar "apps"
  SRCS "epipolar.cpp"
  ITEMS wx3rd_party core dataio epipolar epipolarmodel marshaller
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY})
watxtract_add_executable(wxgeoinfo "apps"
  SRCS "geoinfo.cpp"
  ITEMS wx3rd_party core dataio
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY})

watxtract_install_target(wxepipolar)
watxtract_install_target(wxgeoinfo)
