project(tests_cppunit)

function(add_cppunit_test name)
  earthxpert_add_test(${name} "tests_cppunit" ${ARGN})
endfunction()

add_definitions(-D_USE_MATH_DEFINES)

add_subdirectory(blockadjust)
add_subdirectory(core)
add_subdirectory(epipolarmodel)
#add_subdirectory(filter)
add_subdirectory(io)
#add_subdirectory(marshaller)
#add_subdirectory(rectification)
add_subdirectory(sensormodel)
#add_subdirectory(sfm)
