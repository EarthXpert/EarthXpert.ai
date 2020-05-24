project(sensormodel)

add_definitions(-DEXSENSORMODEL_API)
earthxpert_add_library(sensormodel "api"
  SRCS "ads/adsmodel.cpp"
       "frame/framecp.cpp"
       "frame/frameep.cpp"
       "frame/framemodel.cpp"
       "frame/imagemodelpolicy.cpp"
       "pushbroom/pushbroommodel.cpp"
       "pushbroom/scannerep.cpp"
       "pushbroom/scannerepcorr.cpp"
       "pushbroom/scannerimagemodel.cpp"
       "pushbroom/scannerip.cpp"
       "rational/rationalmodel.cpp"
       "rollingshutter/rollingshuttermodel.cpp"
       "utils/distortiongrid.cpp"
       "utils/frameip.cpp"
       "utils/imagemodel.cpp"
       "utils/satellitedp.cpp"
       "utils/stereomodel.cpp"
       "utils/utils.cpp"
       "sensormodel.cpp"
       "sensormodelfactory.cpp"
  ITEMS ex3rd_party core
        ${GDAL_LIBRARY} ${OPENCV_LIBS})

earthxpert_install_target(sensormodel)
