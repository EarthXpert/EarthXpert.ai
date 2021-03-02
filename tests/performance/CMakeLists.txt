project(tests_performance)

function(add_performance_test name)
  earthxpert_add_test(${name} "tests/performance" ${ARGN})
endfunction()

add_subdirectory(3rdparty)
add_subdirectory(core)
add_subdirectory(dem)
add_subdirectory(io)
add_subdirectory(sensormodel)
