project(frameworks CXX)

add_definitions(-DEXFRAMEWORKS_API)
set(frameworks_srcs 
  "pipeline/pipeline.cpp"
  "pipeline/task.cpp"
  "pipeline/tasksschedular.cpp"
  "pipeline/tileslayout.cpp"
  "utils/utils.cpp")
set(frameworks_items ex3rdparty core  
  Boost::boost ${GDAL_LIBRARIES} ${GETTEXT_LIBRARY} 
  ${OpenCV_LIBS})
earthxpert_add_library(frameworks "api"
  SRCS ${frameworks_srcs}
  ITEMS ${frameworks_items})

earthxpert_source_group(${frameworks_srcs})
earthxpert_install_target(frameworks)
