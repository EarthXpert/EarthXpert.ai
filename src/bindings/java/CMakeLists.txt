project(exjava CXX)

# Build a Java API module:
function(exai_add_java_library name libs_dir)
  exai_add_library(${name} ${libs_dir} ${ARGN})

endfunction()

exai_add_java_library(exjava "bindings/java"
  SRCS "exjava.cpp"
  ITEMS ex3rdparty core dataio epipolar epipolarmodel marshaller sensormodel
        Boost::python37 ${GDAL_LIBRARIES} ${GETTEXT_LIBRARY})

exai_install_target(exjava)
