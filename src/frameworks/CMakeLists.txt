project(frameworks CXX)

add_definitions(-DEXFRAMEWORKS_API)
set(frameworks_srcs 
  "pipeline/pipeline.cpp"
  "pipeline/task.cpp"
  "pipeline/tasksschedular.cpp"
  "pipeline/tileslayout.cpp"
  "utils/utils.cpp")
set(frameworks_items ex3rdparty core  
  Boost::boost ${GDAL_LIBRARIES} glog::glog
  ${OpenCV_LIBS})
exai_add_library(frameworks "api"
  SRCS ${frameworks_srcs}
  ITEMS ${frameworks_items})

exai_source_group(${frameworks_srcs})
exai_install_target(frameworks)
