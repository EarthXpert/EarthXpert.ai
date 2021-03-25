project(colorbal CXX)

add_definitions(-DEXCOLORBAL_API)
set(colorbal_srcs 
  "./cbperf.cpp"
  "./colorbal.cpp"
  "./colorbalqc.cpp")
set(colorbal_items ex3rdparty dataio marshaller 
  ${GDAL_LIBRARIES} ${GETTEXT_LIBRARY} ${OpenCL_LIBS} ${OpenCV_LIBS})
exai_add_library(colorbal "api"
  SRCS ${colorbal_srcs}
  ITEMS ${colorbal_items})

exai_source_group(${colorbal_srcs})
exai_install_target(colorbal)
