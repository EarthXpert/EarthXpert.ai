# Compute locations from <prefix>/@{LIBRARY_DIR@/cmake/<self>.cmake
get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set(EXAI_VERSION_MAJOR 2)
set(EXAI_VERSION_MINOR 0)
set(EXAI_VERSION_PATCH 0)

set(EXAI_BINARY_DIR)
set_and_check(_foo ${PACKAGE_PREFIX_DIR}/bin)
list(APPEND EXAI_BINARY_DIR ${_foo})

set(EXAI_INCLUDE_DIR)
set_and_check(_foo ${PACKAGE_PREFIX_DIR}/include)
list(APPEND EXAI_INCLUDE_DIR ${_foo})

set(EXAI_LIBRARY_DIR)
set_and_check(_foo ${PACKAGE_PREFIX_DIR}/lib)
list(APPEND EXAI_LIBRARY_DIR ${_foo})

include("${CMAKE_CURRENT_LIST_DIR}/EXAITargets.cmake")

set(EXAI_LIBRARIES 
  EXAI::3rdparty 
  EXAI::blockadjust 
  EXAI::colorbal 
  EXAI::core 
  EXAI::dataio 
  EXAI::epipolar 
  EXAI::epiolarmodel 
  EXAI::exgui 
  EXAI::exproject 
  EXAI::expyext 
  EXAI::exutils 
  EXAI::imageproc 
  EXAI::marshaller 
  EXAI::pcproc 
  EXAI::pfexai 
  EXAI::pfgdal 
  EXAI::pfopencv 
  EXAI::pfpdal 
  EXAI::pfservices 
  EXAI::pfutils 
  EXAI::sensormodel
  EXAI::sfm)

check_required_components(EXAI)
