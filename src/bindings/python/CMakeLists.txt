project(expyext CXX)

# Build a Python extension API module:
function(exai_add_py_library name libs_dir)
  exai_add_library(${name} ${libs_dir} ${ARGN})

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

add_definitions(-DEXPYEXT_API)
set(expyext_srcs
  "blockadjust.cpp"
  "core.cpp"
  "epipolar.cpp"
  "expy.cpp"
  "imageproc.cpp"
  "io.cpp"
  "marshaller.cpp"
  "pcproc.cpp"
  "sensormodel.cpp"
  "sfm.cpp"
  "utils.cpp")
set(expyext_items ex3rdparty blockadjust colorbal core dataio 
  epipolar epipolarmodel exproject exutils frameworks imageproc 
  marshaller ortho pcproc sensormodel sfm tortho
  Boost::boost Boost::numpy Boost::python 
  ${GDAL_LIBRARIES} ${geotrans_LIBRARIES} ${GETTEXT_LIBRARY} 
  ${OpenCV_LIBS} ${proj_LIBRARIES} ${PYTHON_LIBRARIES} ${PYTHON_NUMPY_LIBRARIES})
exai_add_py_library(expyext "bindings/python"
  SRCS ${expyext_srcs}
  ITEMS ${expyext_items})

exai_install_target(expyext)
