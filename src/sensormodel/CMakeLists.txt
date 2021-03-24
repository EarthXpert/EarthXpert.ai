project(sensormodel CXX)

add_definitions(-DEXSENSORMODEL_API)
set(sensormodel_srcs "ads/adsmodel.cpp"
  "ads/adsstereomodel.cpp"
  "ads/adsutils.cpp"
  "affine/affinemodel.cpp"
  "frame/fmutils.cpp"
  "frame/framecp.cpp"
  "frame/frameep.cpp"
  "frame/framemodel.cpp"
  "frame/framestereomodel.cpp"
  "frame/imagemodelpolicy.cpp"
  "insar/insarstereomodel.cpp"
  "network/networkmodel.cpp"
  "pushbroom/pbutils.cpp"
  "pushbroom/pushbroommodel.cpp"
  "pushbroom/scannerep.cpp"
  "pushbroom/scannerepcorr.cpp"
  "pushbroom/scannerimagemodel.cpp"
  "pushbroom/scannerip.cpp"
  "rational/rationalmodel.cpp"
  "rational/rationalmodel_cuda.cpp"
  "rational/rationalmodel_opencl.cpp"
  "rational/rationalmodel_simd.cpp"
  "rational/rationalmodelfactory.cpp"
  "rational/rationalstereomodel.cpp"
  "rational/rmutils.cpp"
  "rollingshutter/rollingshuttermodel.cpp"
  "rollingshutter/rsutils.cpp"
  "sar/sarmodel.cpp"
  "sar/sarstereomodel.cpp"
  "utils/evalsensormodel.cpp"
  "utils/evalstereomodel.cpp"
  "utils/distortiongrid.cpp"
  "utils/frameip.cpp"
  "utils/imagemodel.cpp"
  "utils/intersecti2g.cpp"
  "utils/satellitedp.cpp"
  "utils/sensorcube.cpp"
  "utils/smutils.cpp"
  "utils/stereomodel.cpp"
  "utils/stereomodelfactory.cpp"
  "utils/tiledsensormodel.cpp"
  "utils/transsensormodel.cpp"
  "./sensormodel.cpp"
  "./sensormodelfactory.cpp")
set(sensormodel_items ex3rdparty core
  ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} ${OpenCV_LIBS})
exai_add_library(sensormodel "api"
  SRCS ${sensormodel_srcs}
  ITEMS ${sensormodel_items})

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sensormodel_srcs})
exai_source_group(${sensormodel_srcs})
exai_install_target(sensormodel)
