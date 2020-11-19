project(src)

configure_file(srcconfig.h.in ${CMAKE_BINARY_DIR}/src/srcconfig.h)
include_directories(${CMAKE_BINARY_DIR}/src/)

function(earthxpert_source_group)
  foreach(SRC IN ITEMS ${ARGN})
    if(IS_ABSOLUTE ${SRC})
      file(RELATIVE_PATH SRC_REL ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
    else()
        set(SRC_REL ${SRC})
    endif()
    get_filename_component(SRC_PATH ${SRC_REL} PATH)
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
      string(REPLACE "/" "\\" SRC_PATH "${SRC_PATH}")
    endif()
    source_group(${SRC_PATH} FILES ${SRC})
  endforeach()
endfunction(earthxpert_source_group)

function(earthxpert_add_library name libs_dir)
  cmake_parse_arguments(
    EX              # prefix of output variables
    ""              # list of names of the boolean arguments (only defined ones will be true)
    ""              # list of names of mono-valued arguments
    "SRCS;ITEMS"    # list of names of multi-valued arguments (output variables are lists)
    ${ARGN}         # arguments to parse
  )
  
  add_library(${name} SHARED ${EX_SRCS})
  set_target_properties(${name} 
    PROPERTIES
      CXX_STANDARD 17
      DEBUG_POSTFIX "${EARTHXPERT_DEBUG_POSTFIX}")
  target_link_libraries(${name} ${EX_ITEMS} ${vld_LIBRARIES})
  
  if(libs_dir)
    set_property(TARGET ${name} PROPERTY FOLDER ${libs_dir})
  endif()
endfunction()

function(earthxpert_add_executable name exec_dir)
  cmake_parse_arguments(
    EX              # prefix of output variables
    ""              # list of names of the boolean arguments (only defined ones will be true)
    ""              # list of names of mono-valued arguments
    "SRCS;ITEMS"    # list of names of multi-valued arguments (output variables are lists)
    ${ARGN}         # arguments to parse
  )
  
  add_executable(${name} ${EX_SRCS})
  set_target_properties(${name} 
    PROPERTIES
      CXX_STANDARD 17
      DEBUG_POSTFIX "${EARTHXPERT_DEBUG_POSTFIX}")
  target_link_libraries(${name} ${EX_ITEMS} ${vld_LIBRARIES})
  
  if(exec_dir)
    set_property(TARGET ${name} PROPERTY FOLDER ${exec_dir})
  endif()
endfunction()

function(earthxpert_install_target name)
  install(TARGETS ${name} ${ARGN}
    LIBRARY DESTINATION sdk/lib
    ARCHIVE DESTINATION sdk/lib
    RUNTIME DESTINATION ${EARTHXPERT_BIN_DIR})
  if(WIN32 AND MSVC AND (NOT MSVC_VERSION LESS 1600))
    install(FILES $<TARGET_PDB_FILE:${name}> DESTINATION ${EARTHXPERT_BIN_DIR} OPTIONAL)
  endif()
endfunction()

add_compile_definitions(BOOST_LIB_TOOLSET="vc142")
add_definitions(-D_USE_MATH_DEFINES -DBOOST_ALL_DYN_LINK)

add_subdirectory(3rdparty)
add_subdirectory(app)
add_subdirectory(bindings)
add_subdirectory(blockadjust)
add_subdirectory(core)
add_subdirectory(epipolarmodel)
add_subdirectory(gui)
add_subdirectory(imageproc)
add_subdirectory(io)
add_subdirectory(marshaller)
add_subdirectory(packages)
add_subdirectory(pcproc)
add_subdirectory(project)
add_subdirectory(rectification)
add_subdirectory(sensormodel)
add_subdirectory(services)
add_subdirectory(sfm)
add_subdirectory(utils)
