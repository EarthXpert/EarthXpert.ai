project(tests_performance)

function(add_performance_test name)
  watxtract_add_test(${name} "tests_performance" ${ARGN})
endfunction()

add_definitions(-D_USE_MATH_DEFINES)

add_subdirectory(io)
