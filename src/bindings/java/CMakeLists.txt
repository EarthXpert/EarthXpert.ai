project(exjava CXX)

# Build a Java API module:
function(earthxpert_add_java_library name libs_dir)
  earthxpert_add_library(${name} ${libs_dir} ${ARGN})

endfunction()

earthxpert_add_java_library(exjava "bindings"
  SRCS "exjava.cpp"
  ITEMS ex3rdparty core dataio epipolar epipolarmodel marshaller sensormodel
        Boost::python37 ${GDAL_LIBRARY} ${GETTEXT_LIBRARY})

earthxpert_install_target(exjava)
