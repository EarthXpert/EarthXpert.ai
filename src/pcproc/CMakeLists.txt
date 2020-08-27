project(pcproc CXX)

add_definitions(-DEXPCPROC_API)
set(pcproc_srcs "filter/pcfilter.cpp"
  "filter/pmfilter.cpp")
earthxpert_add_library(pcproc "api"
  SRCS ${pcproc_srcs}
  ITEMS ex3rdparty core  
        ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} ${OpenCL_LIBS} ${OpenCV_LIBS})

earthxpert_source_group(${pcproc_srcs})
earthxpert_install_target(pcproc)
