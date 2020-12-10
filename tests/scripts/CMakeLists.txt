project(tests_scripts)

function(add_scripts_test name)
  earthxpert_add_test(${name} "tests/scripts" ${ARGN})
endfunction()

#add_subdirectory()
