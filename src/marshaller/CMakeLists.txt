project(marshaller)

add_definitions(-DEXMARSHALLER_API -D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
set(marshaller_srcs "epipolarmodel/epipolarstereomarshaller.cpp"
       "project/projectmarshaller.cpp"
       "project/p4dprjmarshaller.cpp"
       "sensormodel/distgridmarshaller.cpp"
       "sensormodel/framemodelmarshaller.cpp"
       "sensormodel/rationalmodelmarshaller.cpp")
earthxpert_add_library(marshaller "api"
  SRCS ${marshaller_srcs}
  ITEMS ex3rdparty core epipolarmodel exproject sensormodel
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${LIBXML2_LIBRARIES} )

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${marshaller_srcs})
earthxpert_source_group(${marshaller_srcs})
earthxpert_install_target(marshaller)
