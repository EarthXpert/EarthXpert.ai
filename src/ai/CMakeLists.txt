project(ai CXX)

add_definitions(-DEXAI_API)
set(ai_srcs 
  "ann/ann.cpp"
  "svm/svm.cpp"
  "utils/utils.cpp")
set(ai_items ex3rdparty Boost::boost Boost::filesystem Boost::locale 
  glog::glog ${LIBXML2_LIBRARIES} ${OpenCV_LIBS} 
  ${OpenEXR_LIBS})
exai_add_library(ai "api"
  SRCS ${ai_srcs}
  ITEMS ${ai_items})

exai_source_group(${ai_srcs})
exai_install_target(ai)
