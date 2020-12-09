project(frameworks CXX)

add_definitions(-DEXFRAMEWORKS_API)
set(frameworks_srcs "pipeline/tasksschedular.cpp"
  "pipeline/tileslayout.cpp"
  "utils/utils.cpp")
earthxpert_add_library(frameworks "api"
  SRCS ${frameworks_srcs}
  ITEMS ex3rdparty core  
        Boost::boost ${GDAL_LIBRARY} ${GETTEXT_LIBRARY} 
        ${OpenCV_LIBS})

earthxpert_source_group(${frameworks_srcs})
earthxpert_install_target(frameworks)
