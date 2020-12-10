project(app)

function(add_cli_executable name)
  earthxpert_add_executable(${name} "apps/cli" ${ARGN})
endfunction()

function(add_gui_executable name)
  earthxpert_add_executable(${name} "apps/gui" ${ARGN})
endfunction()

add_subdirectory(cli)
add_subdirectory(gui)
