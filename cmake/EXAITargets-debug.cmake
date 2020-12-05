#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "core" for configuration "Debug"
set_property(TARGET EXAI::core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(EXAI::core PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/cored.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/debug/cored.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::core )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::core "${_IMPORT_PREFIX}/lib/cored.lib" "${_IMPORT_PREFIX}/bin/debug/cored.dll" )

# Import target "dataio" for configuration "Debug"
set_property(TARGET EXAI::dataio APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(EXAI::dataio PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/dataiod.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/debug/dataiod.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::dataio )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::dataio "${_IMPORT_PREFIX}/lib/dataiod.lib" "${_IMPORT_PREFIX}/bin/debug/dataiod.dll" )

# Import target "epipolar" for configuration "Debug"
set_property(TARGET EXAI::epipolar APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(EXAI::epipolar PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/epipolard.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/debug/epipolard.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::epipolar )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::epipolar "${_IMPORT_PREFIX}/lib/epipolard.lib" "${_IMPORT_PREFIX}/bin/debug/epipolard.dll" )

# Import target "epipolarmodel" for configuration "Debug"
set_property(TARGET EXAI::epipolarmodel APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(EXAI::epipolarmodel PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/epipolarmodeld.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/debug/epipolarmodeld.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::epipolarmodel )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::epipolarmodel "${_IMPORT_PREFIX}/lib/epipolarmodeld.lib" "${_IMPORT_PREFIX}/bin/debug/epipolarmodeld.dll" )

# Import target "marshaller" for configuration "Debug"
set_property(TARGET EXAI::marshaller APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(EXAI::marshaller PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/marshallerd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/debug/marshallerd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::marshaller )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::marshaller "${_IMPORT_PREFIX}/lib/marshallerd.lib" "${_IMPORT_PREFIX}/bin/debug/marshallerd.dll" )

# Import target "sensormodel" for configuration "Debug"
set_property(TARGET EXAI::sensormodel APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(EXAI::sensormodel PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/sensormodeld.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/debug/sensormodeld.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::sensormodel )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::sensormodel "${_IMPORT_PREFIX}/lib/sensormodeld.lib" "${_IMPORT_PREFIX}/bin/debug/sensormodeld.dll" )

# Import target "ex3rdparty" for configuration "Debug"
set_property(TARGET EXAI::ex3rdparty APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(EXAI::ex3rdparty PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/ex3rdpartyd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/debug/ex3rdpartyd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::ex3rdparty )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::ex3rdparty "${_IMPORT_PREFIX}/lib/ex3rdpartyd.lib" "${_IMPORT_PREFIX}/bin/debug/ex3rdpartyd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
