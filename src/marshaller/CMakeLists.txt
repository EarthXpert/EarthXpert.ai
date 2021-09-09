project(marshaller CXX)

add_definitions(-DEXMARSHALLER_API -D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
set(marshaller_srcs 
  "epipolarmodel/epipolarstereomarshaller.cpp"
  "project/adsprjmarshaller.cpp"
  "project/bgoprjmarshaller.cpp"
  "project/oeprjmarshaller.cpp"
  "project/p4dprjmarshaller.cpp"
  "project/projectmarshaller.cpp"
  "project/ratprjmarshaller.cpp"
  "project/sarprjmarshaller.cpp"
  "sensormodel/distgridmarshaller.cpp"
  "sensormodel/framemodelmarshaller.cpp"
  "sensormodel/rationalmodelmarshaller.cpp"
  "sfm/featuremarshaller.cpp"
  "support/adssupportmarshaller.cpp"
  "support/supportmarshaller.cpp"
  "utils/utils.cpp")
set(marshaller_items ex3rdparty core epipolarmodel exproject sensormodel
  ${GDAL_LIBRARIES} glog::glog ${ICONV_LIBRARIES} ${LIBXML2_LIBRARIES})
exai_add_library(marshaller "api"
  SRCS ${marshaller_srcs}
  ITEMS ${marshaller_items})

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${marshaller_srcs})
exai_source_group(${marshaller_srcs})
exai_install_target(marshaller)
