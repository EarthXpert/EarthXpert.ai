project(tests_regression)

function(add_regression_test name)
  earthxpert_add_test(${name} "tests/regression" ${ARGN})
endfunction()

add_definitions(-D_USE_MATH_DEFINES)

#add_subdirectory()
