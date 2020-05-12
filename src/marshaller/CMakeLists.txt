project(marshaller)

add_definitions(-DWXMARSHALLER_API -D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
watxtract_add_library(marshaller "api"
  SRCS "epipolarmodel/epipolarmodelmarshaller.cpp"
       "epipolarmodel/epipolarrectmarshaller.cpp"
       "sensormodel/distgridmarshaller.cpp"
       "sensormodel/framemodelmarshaller.cpp"
       "sensormodel/rationalmodelmarshaller.cpp"
  ITEMS wx3rd_party core sensormodel epipolarmodel
        ${GDAL_LIBRARY})

watxtract_install_target(marshaller)
