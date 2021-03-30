project(pcproc CXX)

add_definitions(-DEXPCPROC_API)
set(pcproc_srcs 
  "./laslibutils.cpp"
  "./pdalutils.cpp"
  "filter/filterqc.cpp"
  "filter/pcfilter.cpp"
  "filter/pmfilter.cpp"
  "filter/pmfilter_pdal.cpp")
set(pcproc_items ex3rdparty core  
  ${GDAL_LIBRARIES} ${GETTEXT_LIBRARY} ${OpenCL_LIBS} ${OpenCV_LIBS})
exai_add_library(pcproc "api"
  SRCS ${pcproc_srcs}
  ITEMS ${pcproc_items})

exai_source_group(${pcproc_srcs})
exai_install_target(pcproc)
