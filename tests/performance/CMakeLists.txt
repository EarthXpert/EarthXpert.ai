project(tests_performance)

function(add_performance_test name)
  earthxpert_add_test(${name} "tests_performance" ${ARGN})
endfunction()

add_definitions(-D_USE_MATH_DEFINES)

add_subdirectory(core)
add_subdirectory(io)
add_subdirectory(sensormodel)
add_subdirectory(stl)
