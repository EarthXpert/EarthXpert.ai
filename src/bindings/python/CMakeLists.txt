project(expyext CXX)

include_directories(${pybind11_INCLUDE_DIRS}) 
include_directories(${PYTHON_INCLUDE_DIRS}) 

# Build a Python extension API module:
function(earthxpert_add_py_library name libs_dir)
  earthxpert_add_library(${name} ${libs_dir} ${ARGN})

  # Python debug libraries expose slightly different objects
  # https://docs.python.org/3.6/c-api/intro.html#debugging-builds
  # https://stackoverflow.com/questions/39161202/how-to-work-around-missing-pymodule-create2-in-amd64-win-python35-d-lib
  if(PYTHON_IS_DEBUG)
    target_compile_definitions(${name} PRIVATE Py_DEBUG)
  endif()

  set_target_properties(${name} 
    PROPERTIES
      CXX_VISIBILITY_PRESET "hidden"
      CUDA_VISIBILITY_PRESET "hidden"
      PREFIX "${PYTHON_MODULE_PREFIX}"
      SUFFIX "${PYTHON_MODULE_EXTENSION}")
endfunction()

earthxpert_add_py_library(expyext "bindings/python"
  SRCS "expyext.cpp"
  ITEMS ex3rdparty core dataio epipolar epipolarmodel marshaller sensormodel
        Boost::python37 ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${PYTHON_LIBRARIES})

earthxpert_install_target(expyext)
