########################################################################
#
# Helper functions for creating build targets.

# local_occutils_test_cxx_executable_with_flags(name cxx_flags lib srcs...)
#
# creates a named C++ executable that depends on the given libraries and
# is built from the given source files with the given compiler flags.
function (local_occutils_test_cxx_executable_with_flags name cxx_flags libs)
  add_executable(${name} ${ARGN})
  if (cxx_flags)
    set_target_properties(${name}
                          PROPERTIES
                          COMPILE_FLAGS "${cxx_flags}")
  endif ()
  # To support mixing linking in static and dynamic libraries, link each
  # library in with an extra call to target_link_libraries.
  foreach (lib ${libs})
    target_link_libraries(${name} ${lib})
  endforeach ()
endfunction ()

# occutils_test_cxx_executable(name srcs lib...)
#
# creates a named target that is built from the given source files and depends
# on the given lib.
function (occutils_test_cxx_executable name srcs libs)
  local_occutils_test_cxx_executable_with_flags(
    ${name} "${cxx_default}" "${libs}" "${srcs}" ${ARGN})
endfunction ()
