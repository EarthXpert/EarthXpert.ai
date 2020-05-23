# top-level CMake configuration file for EarthXpert.ai
cmake_minimum_required(VERSION 3.12)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
if(POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

project(earthxpert)
set(EARTHXPERT_VERSION 1.0.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/sdk/lib)
set(EARTHXPERT_WORKSPACE_DIR "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/../>")
message(STATUS "EARTHXPERT_WORKSPACE_DIR: ${EARTHXPERT_WORKSPACE_DIR}")

set(EX_THIRD_PARTY "$ENV{EX_Third_Party}")

# section third-party
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if (MSVC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W1")
  endif()
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DCPL_DISABLE_DLL)

  set(EARTHXPERT_DEBUG_POSTFIX d)
  set(CDEGREL $<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>)
  message(STATUS "Debug_Release_Mode: ${CDEGREL}")

  set(EARTHXPERT_BIN_ROOT "bin")
  set(EARTHXPERT_BIN_DIR "${EARTHXPERT_BIN_ROOT}/${CDEGREL}")

  set(BOOST_ROOT "${EX_THIRD_PARTY}/boost_1.71.0")

  set(CEREAL_INCLUDE_DIR "${EX_THIRD_PARTY}/cereal_1.2.2/include")

  set(GDAL_ROOT "${EX_THIRD_PARTY}/gdal_2.4.1")
  set(GDAL_BIN_DIR "${GDAL_ROOT}/bin/${CDEGREL}")
  set(GDAL_INCLUDE_DIR "${GDAL_ROOT}/include")
  set(GDAL_LIBRARY "${GDAL_ROOT}/lib/${CDEGREL}/gdal_i.lib")

  set(GTEST_ROOT "${EX_THIRD_PARTY}/gtest_1.8.1")
  set(GTEST_BIN_DIR "${GTEST_ROOT}/bin")
  set(GTEST_INCLUDE_DIR "${GTEST_ROOT}/include")
  
  set(HDF5_ROOT "${EX_THIRD_PARTY}/hdf5_1.10.5")
  set(HDF5_BIN_DIR "${HDF5_ROOT}/bin")
  
  set(LASLIB_ROOT "${EX_THIRD_PARTY}/lastools_19.05.07")
  #set(LASLIB_BIN_DIR "${LASLIB_ROOT}/bin")
  set(LASLIB_INCLUDE_DIR "${LASLIB_ROOT}/include/LASlib")
  set(LASLIB_LIBRARY "${LASLIB_ROOT}/lib/${CDEGREL}/LASlib/LASlib.lib")

  set(OPENCV_ROOT "${EX_THIRD_PARTY}/opencv_4.1.0")
  set(OpenCV_DIR "${OPENCV_ROOT}")
  set(OPENCV_BIN_DIR "${OPENCV_ROOT}/x64/vc15/bin")
  set(OPENCV_INCLUDE_DIR "${OPENCV_ROOT}/include")
  set(OPENCV_LIBRARY_DIR "${OPENCV_ROOT}/x64/vc15/lib")
  set(OPENCV_LIBS debug "${OPENCV_LIBRARY_DIR}/opencv_img_hash410d.lib"
                  debug "${OPENCV_LIBRARY_DIR}/opencv_world410d.lib"
                  optimized "${OPENCV_LIBRARY_DIR}/opencv_img_hash410.lib"
                  optimized "${OPENCV_LIBRARY_DIR}/opencv_world410.lib")

  set(OPENNI_ROOT "${EX_THIRD_PARTY}/openni_2.2.0")
  set(OPENNI_BIN_DIR "${OPENNI_ROOT}/Redist")

  set(pybind11_DIR "${EX_THIRD_PARTY}/pybind11_2.5.0/share/cmake/pybind11")

  set(Qt5_DIR "${EX_THIRD_PARTY}/qt_5.11.2/lib/cmake/Qt5")
  
  set(Sltbench_DIR "${EX_THIRD_PARTY}/sltbench_20.01.15/cmake")
  
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(EARTHXPERT_BIN_ROOT "bin")
  set(EARTHXPERT_BIN_DIR "bin")
  set(EARTHXPERT_DEBUG_POSTFIX "")

  set(CEREAL_INCLUDE_DIR "${EX_THIRD_PARTY}/cereal_1.2.2/include")

  set(GDAL_ROOT "${EX_THIRD_PARTY}/gdal_2.3.2")
  set(GDAL_INCLUDE_DIR "${GDAL_ROOT}/include")
  set(GDAL_LIBRARY "${GDAL_ROOT}/lib/libgdal.so")

  set(GTEST_ROOT "${EX_THIRD_PARTY}/gtest_1.8.1")
  set(GTEST_INCLUDE_DIR "${GTEST_ROOT}/include")
  
  set(LASLIB_ROOT "${EX_THIRD_PARTY}/lastools_19.01.27")
  set(LASLIB_INCLUDE_DIR "${LASLIB_ROOT}/include/LASlib")
  set(LASLIB_LIBRARY "${LASLIB_ROOT}/lib/LASlib/libLASlib.a")

  set(OPENCV_ROOT "${EX_THIRD_PARTY}/opencv_4.1.0")
  set(OPENCV_DIR "${OPENCV_ROOT}/lib/cmake/opencv4")
  set(OPENCV_INCLUDE_DIR "${OPENCV_ROOT}/include/opencv4")
  set(OPENCV_LIBRARY_DIR "${OPENCV_ROOT}/lib")
  set(OPENCV_LIBS "${OPENCV_LIBRARY_DIR}/libopencv_img_hash.so"
                  "${OPENCV_LIBRARY_DIR}/libopencv_sfm.so"
                  "${OPENCV_LIBRARY_DIR}/libopencv_world.so")
                  
else()
  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME}: not supported.")
endif()

message(STATUS "EARTHXPERT_BIN_DIR: ${EARTHXPERT_BIN_DIR}")

if(DEFINED CMAKE_DEBUG_POSTFIX)
  set(EARTHXPERT_DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
endif()

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.71.0 COMPONENTS filesystem iostreams program_options python37) 

find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(GDAL REQUIRED)
find_package(GTest REQUIRED)
find_package(OpenCV REQUIRED)
find_package(pybind11 REQUIRED)
find_package(Qt5 5.11.2 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Sltbench REQUIRED)

get_target_property(SLTBENCH_INCLUDE_DIRS sltbench INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SLTBENCH_LIBRARY_DEBUG sltbench IMPORTED_LOCATION_DEBUG)
get_target_property(SLTBENCH_LIBRARY_RELEASE sltbench IMPORTED_LOCATION_RELEASE)
set(SLTBENCH_LIBS debug ${SLTBENCH_LIBRARY_DEBUG}
                  optimized ${SLTBENCH_LIBRARY_RELEASE})

# remark: BOOST_BIN_DIR is empty in subdirectory!
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(BOOST_BIN_DIR "${Boost_DIR}/../..")
  message(STATUS "BOOST_BIN_DIR: ${BOOST_BIN_DIR}")
else()

endif()

# section include
include_directories(${Boost_INCLUDE_DIR}) 
include_directories(${CEREAL_INCLUDE_DIR})
include_directories(${GDAL_INCLUDE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${LASLIB_INCLUDE_DIR})
include_directories(${OPENCV_INCLUDE_DIR})
include_directories(${SLTBENCH_INCLUDE_DIRS})

include_directories("include")

# section source
add_subdirectory(python)
add_subdirectory(src)
enable_testing()
add_subdirectory(tests)

# section install
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  install(DIRECTORY ${GDAL_ROOT}/bin/debug/ 
          DESTINATION bin/debug
          CONFIGURATIONS Debug
          FILES_MATCHING PATTERN "*.dll" PATTERN "*.pdb")
  install(DIRECTORY ${GDAL_ROOT}/bin/release/ 
          DESTINATION bin/release
          CONFIGURATIONS Release
          FILES_MATCHING PATTERN "*.dll")
  #install(DIRECTORY ${HDF5_BIN_DIR}/
  #        DESTINATION bin 
  #        FILES_MATCHING PATTERN "*.dll")
  #install(DIRECTORY ${OPENCV_BIN_DIR}/
  #            DESTINATION bin 
  #        FILES_MATCHING PATTERN "*.dll" PATTERN "*.pdb")
  #install(DIRECTORY ${QT_BIN_DIR}/
  #        DESTINATION bin 
  #        FILES_MATCHING PATTERN "*.dll")

  set(GDAL_DATA "${GDAL_ROOT}/data/")
  set(OPENCV_DATA "${OPENCV_ROOT}/etc/")
else()
  # TODO
  set(GDAL_DATA "${GDAL_ROOT}/share/gdal/")
  set(OPENCV_DATA "${OPENCV_ROOT}/share/opencv4/")
  set(OPENCV_LICENSES "${OPENCV_ROOT}/share/licenses/opencv4/")
endif()

install(DIRECTORY include DESTINATION sdk)
install(DIRECTORY ${GDAL_DATA} DESTINATION etc/gdal)
install(DIRECTORY ${OPENCV_DATA} DESTINATION etc/opencv)

if(OPENCV_LICENSES)
  install(DIRECTORY ${OPENCV_LICENSES} DESTINATION etc/opencv/licenses)
endif()

option(EARTHXPERT_INSTALL_PACKAGES_3RD_PARTY "install 3rd-party packages" OFF)
if(EARTHXPERT_INSTALL_PACKAGES_3RD_PARTY)
endif()
