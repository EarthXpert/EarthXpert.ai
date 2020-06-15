project(tools)

earthxpert_add_executable(exepipolar "apps"
  SRCS "epipolar.cpp"
  ITEMS ex3rdparty core dataio epipolar epipolarmodel marshaller
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY})
earthxpert_add_executable(exgeoinfo "apps"
  SRCS "geoinfo.cpp"
  ITEMS ex3rdparty core dataio
        Boost::filesystem Boost::program_options
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY})

earthxpert_install_target(exepipolar)
earthxpert_install_target(exgeoinfo)
