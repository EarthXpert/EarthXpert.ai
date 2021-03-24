project(exdem CXX)

add_definitions(-DEXDEM_API)
set(exdem_srcs "./dem.cpp"
  "./demfactory.cpp"
  "./geoidfactory.cpp"
  "geoid/geoid.cpp"
  "geoid/localgeoid.cpp"
  "rasterdem/rasterdem.cpp"
  "tindem/tindem.cpp"
  "utils/demqc.cpp"
  "utils/landscapemodel.cpp")
set(exdem_items ex3rdparty core dataio
  Boost::boost Boost::filesystem Boost::iostreams
  ${CERES_LIBRARY} ${CGAL_LIBRARIES} ${GDAL_LIBRARIES} ${geotrans_LIBRARIES}
  ${GETTEXT_LIBRARY} ${LASLIB_LIBRARY} ${OpenCV_LIBS})
exai_add_library(exdem "api"
  SRCS ${exdem_srcs}
  ITEMS ${exdem_items})

exai_source_group(${exdem_srcs})
exai_install_target(exdem)
