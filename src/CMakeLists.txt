project(src)

configure_file(srcconfig.h.in ${CMAKE_BINARY_DIR}/src/srcconfig.h)
include_directories(${CMAKE_BINARY_DIR}/src/)

function(watxtract_add_library name libs_dir)
  cmake_parse_arguments(
    WX              # prefix of output variables
    ""              # list of names of the boolean arguments (only defined ones will be true)
    ""              # list of names of mono-valued arguments
    "SRCS;ITEMS"    # list of names of multi-valued arguments (output variables are lists)
    ${ARGN}         # arguments to parse
  )
  
  add_library(${name} SHARED ${WX_SRCS})
  set_target_properties(${name} 
    PROPERTIES
      CXX_STANDARD 17
      DEBUG_POSTFIX "${WATXTRACT_DEBUG_POSTFIX}")
  target_link_libraries(${name} ${WX_ITEMS})
  
  if(libs_dir)
    set_property(TARGET ${name} PROPERTY FOLDER ${libs_dir})
  endif()
endfunction()

function(watxtract_add_executable name exec_dir)
  cmake_parse_arguments(
    WX              # prefix of output variables
    ""              # list of names of the boolean arguments (only defined ones will be true)
    ""              # list of names of mono-valued arguments
    "SRCS;ITEMS"    # list of names of multi-valued arguments (output variables are lists)
    ${ARGN}         # arguments to parse
  )
  
  add_executable(${name} ${WX_SRCS})
  set_target_properties(${name} 
    PROPERTIES
      CXX_STANDARD 17
      DEBUG_POSTFIX "${WATXTRACT_DEBUG_POSTFIX}")
  target_link_libraries(${name} ${WX_ITEMS})
  
  if(exec_dir)
    set_property(TARGET ${name} PROPERTY FOLDER ${exec_dir})
  endif()
endfunction()

function(watxtract_install_target name)
  install(TARGETS ${name} ${ARGN}
    LIBRARY DESTINATION sdk/lib
    ARCHIVE DESTINATION sdk/lib
    RUNTIME DESTINATION ${WATXTRACT_BIN_DIR})
  if(WIN32 AND MSVC AND (NOT MSVC_VERSION LESS 1600))
    install(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${WATXTRACT_BIN_DIR} OPTIONAL)
  endif()
endfunction()

add_definitions(-D_USE_MATH_DEFINES -DBOOST_ALL_DYN_LINK)
add_subdirectory(3rd_party)
add_subdirectory(app)
add_subdirectory(bindings)
add_subdirectory(core)
add_subdirectory(io)
add_subdirectory(sensormodel)
add_subdirectory(epipolarmodel)
add_subdirectory(marshaller)
add_subdirectory(rectification)
add_subdirectory(sfm)
