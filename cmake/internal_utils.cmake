# Defines the occutils library the user should link with
function (cxx_library_with_type name type)
  # type can be either STATIC or SHARED to denote a static or shared library.
  # ARGN refers to additional arguments after 'type'.
  add_library(${name} ${type} ${ARGN})
  add_library(${cmake_package_name}::${name} ALIAS ${name})

  set_target_properties(${name} PROPERTIES DEBUG_POSTFIX d)

  # Set the output directory for build artifacts
  set_target_properties(${name}
                        PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                        PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                        COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  # make PDBs match library name
  get_target_property(pdb_debug_postfix ${name} DEBUG_POSTFIX)
  set_target_properties(${name}
                        PROPERTIES
                        PDB_NAME "${name}"
                        PDB_NAME_DEBUG "${name}${pdb_debug_postfix}"
                        COMPILE_PDB_NAME "${name}"
                        COMPILE_PDB_NAME_DEBUG "${name}${pdb_debug_postfix}")

  if (BUILD_SHARED_LIBS OR type STREQUAL "SHARED")
    set_target_properties(${name}
                          PROPERTIES
                          COMPILE_DEFINITIONS "OCCUTILS_CREATE_SHARED_LIBRARY=1")
    if (NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.11")
      target_compile_definitions(${name} INTERFACE
                                 $<INSTALL_INTERFACE:OCCUTILS_LINKED_AS_SHARED_LIBRARY=1>)
    endif ()
  endif ()
endfunction ()

########################################################################
#
# Helper functions for creating build targets.

function (cxx_shared_library name)
  cxx_library_with_type(${name} SHARED ${ARGN})
endfunction ()

function (cxx_library name)
  cxx_library_with_type(${name} "" ${ARGN})
endfunction ()

########################################################################
#
# Configure compiler warnings

# Turn on warnings on the given target
function (target_enable_warnings target_name)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND MSVC_OPTIONS "/W4")
    if (MSVC_VERSION GREATER 1900) # Allow non fatal security warnings for msvc 2015
      list(APPEND MSVC_OPTIONS "/WX")
    endif ()
  endif ()

  target_compile_options(
    ${target_name}
    PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall
    -Wextra
    -Wconversion
    -pedantic
    -Werror
    -Wfatal-errors>
    $<$<CXX_COMPILER_ID:MSVC>:${MSVC_OPTIONS}>)
endfunction ()
