project(colorbal CXX)

add_definitions(-DEXCOLORBAL_API)
set(colorbal_srcs "./cbperf.cpp"
  "./colorbal.cpp")
earthxpert_add_library(colorbal "api"
  SRCS ${colorbal_srcs}
  ITEMS ex3rdparty dataio marshaller 
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCL_LIBS} ${OpenCV_LIBS})

earthxpert_source_group(${colorbal_srcs})
earthxpert_install_target(colorbal)
