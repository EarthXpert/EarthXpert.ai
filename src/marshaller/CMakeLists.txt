project(marshaller)

add_definitions(-DEXMARSHALLER_API -D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
earthxpert_add_library(marshaller "api"
  SRCS "epipolarmodel/epipolarmodelmarshaller.cpp"
       "project/projectmarshaller.cpp"
       "project/p4dprjmarshaller.cpp"
       "sensormodel/distgridmarshaller.cpp"
       "sensormodel/framemodelmarshaller.cpp"
       "sensormodel/rationalmodelmarshaller.cpp"
  ITEMS ex3rdparty core epipolarmodel exproject sensormodel
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${LIBXML2_LIBRARIES} )

earthxpert_install_target(marshaller)
