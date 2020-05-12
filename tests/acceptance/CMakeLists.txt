project(tests_acceptance)

function(add_acceptance_test name)
  watxtract_add_test(${name} "tests_acceptance" ${ARGN})
endfunction()

add_definitions(-D_USE_MATH_DEFINES)

#add_subdirectory()
