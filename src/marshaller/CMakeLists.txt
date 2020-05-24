project(marshaller)

add_definitions(-DEXMARSHALLER_API -D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
earthxpert_add_library(marshaller "api"
  SRCS "epipolarmodel/epipolarmodelmarshaller.cpp"
       "epipolarmodel/epipolarrectmarshaller.cpp"
       "sensormodel/distgridmarshaller.cpp"
       "sensormodel/framemodelmarshaller.cpp"
       "sensormodel/rationalmodelmarshaller.cpp"
  ITEMS ex3rd_party core sensormodel epipolarmodel
        ${GDAL_LIBRARY})

earthxpert_install_target(marshaller)
