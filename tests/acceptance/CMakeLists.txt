project(tests_acceptance)

function(add_acceptance_test name)
  earthxpert_add_test(${name} "tests/acceptance" ${ARGN})
endfunction()

add_definitions(-D_USE_MATH_DEFINES)

#add_subdirectory()
