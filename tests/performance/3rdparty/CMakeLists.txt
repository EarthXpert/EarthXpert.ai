project(tests_performance_3rdparty)

function(add_3rdparty_perf_test name)
  earthxpert_add_test(${name} "tests/performance/3rdparty" ${ARGN})
endfunction()

add_subdirectory(eigen)
add_subdirectory(stl)
