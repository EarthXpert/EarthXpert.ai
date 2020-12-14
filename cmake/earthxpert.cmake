# CMake utils for EarthXpert.ai

function(earthxpert_get_INCLUDE_DIRS name target_name)
  get_target_property("${name}_INCLUDE_DIRS" ${target_name} INTERFACE_INCLUDE_DIRECTORIES)
endfunction()

function(earthxpert_get_LIBRARIES name target_name)
  get_target_property("${name}_LIBRARY_DEBUG" ${target_name} IMPORTED_LOCATION_DEBUG)
  get_target_property("${name}_LIBRARY_RELEASE" ${target_name} IMPORTED_LOCATION_RELEASE)
  set("${name}_LIBRARIES" debug "${name}_LIBRARY_DEBUG"
                          optimized "${name}_LIBRARY_RELEASE")
endfunction()
