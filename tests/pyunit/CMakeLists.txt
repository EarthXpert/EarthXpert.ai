project(tests_pyunit)

function(add_pyunit_test name)
  exai_add_test(${name} "tests/pyunit" ${ARGN})
endfunction()

add_subdirectory(python2)
add_subdirectory(python3)
