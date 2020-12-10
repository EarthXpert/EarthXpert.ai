#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "core" for configuration "Release"
set_property(TARGET EXAI::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EXAI::core PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/core.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/release/core.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::core )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::core "${_IMPORT_PREFIX}/lib/core.lib" "${_IMPORT_PREFIX}/bin/release/core.dll" )

# Import target "dataio" for configuration "Release"
set_property(TARGET EXAI::dataio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EXAI::dataio PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/dataio.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/release/dataio.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::dataio )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::dataio "${_IMPORT_PREFIX}/lib/dataio.lib" "${_IMPORT_PREFIX}/bin/release/dataio.dll" )

# Import target "epipolar" for configuration "Release"
set_property(TARGET EXAI::epipolar APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EXAI::epipolar PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/epipolar.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/release/epipolar.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::epipolar )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::epipolar "${_IMPORT_PREFIX}/lib/epipolar.lib" "${_IMPORT_PREFIX}/bin/release/epipolar.dll" )

# Import target "epipolarmodel" for configuration "Release"
set_property(TARGET EXAI::epipolarmodel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EXAI::epipolarmodel PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/epipolarmodel.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/release/epipolarmodel.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::epipolarmodel )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::epipolarmodel "${_IMPORT_PREFIX}/lib/epipolarmodel.lib" "${_IMPORT_PREFIX}/bin/release/epipolarmodel.dll" )

# Import target "marshaller" for configuration "Release"
set_property(TARGET EXAI::marshaller APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EXAI::marshaller PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/marshaller.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/release/marshaller.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::marshaller )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::marshaller "${_IMPORT_PREFIX}/lib/marshaller.lib" "${_IMPORT_PREFIX}/bin/release/marshaller.dll" )

# Import target "sensormodel" for configuration "Release"
set_property(TARGET EXAI::sensormodel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EXAI::sensormodel PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/sensormodel.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/release/sensormodel.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::sensormodel )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::sensormodel "${_IMPORT_PREFIX}/lib/sensormodel.lib" "${_IMPORT_PREFIX}/bin/release/sensormodel.dll" )

# Import target "ex3rdparty" for configuration "Release"
set_property(TARGET EXAI::ex3rdparty APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(EXAI::ex3rdparty PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/ex3rdparty.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/release/ex3rdparty.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS EXAI::ex3rdparty )
list(APPEND _IMPORT_CHECK_FILES_FOR_EXAI::ex3rdparty "${_IMPORT_PREFIX}/lib/ex3rdparty.lib" "${_IMPORT_PREFIX}/bin/release/ex3rdparty.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
