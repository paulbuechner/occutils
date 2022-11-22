 # Configure OCCT libraries to link with
 macro (target_link_occt_libraries target)
   foreach (LIB ${OpenCASCADE_LIBRARIES})
     if (WIN32)
       if (EXISTS ${OpenCASCADE_LIBRARY_DIR}${DEBUG_SUFFIX}/${LIB}.lib)
         if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
           set(DEBUG_SUFFIX "d")
         endif ()
         set(occt_lib_dir ${OpenCASCADE_LIBRARY_DIR}${DEBUG_SUFFIX}/${LIB}.lib)
         message(STATUS "Adding ${occt_lib_dir}")

         target_link_libraries(${target} debug ${occt_lib_dir})
         target_link_libraries(${target} optimized ${occt_lib_dir})
       else ()
         message(FATAL_ERROR "Library ${LIB} not found in ${OpenCASCADE_LIBRARY_DIR}${DEBUG_SUFFIX}")
       endif ()
     elseif (APPLE)
       if (EXISTS ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.dylib)
         set(occt_lib_dir ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.dylib)
         message(STATUS "Adding ${occt_lib_dir}")

         target_link_libraries(${target} debug ${occt_lib_dir})
         target_link_libraries(${target} optimized ${occt_lib_dir})
       else ()
         message(FATAL_ERROR "Library lib${LIB} not found in ${OpenCASCADE_LIBRARY_DIR}")
       endif ()
     elseif (UNIX)
       if (EXISTS ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so)
         set(occt_lib_dir ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so)
         message(STATUS "Adding ${occt_lib_dir}")

         target_link_libraries(${target} debug ${occt_lib_dir})
         target_link_libraries(${target} optimized ${occt_lib_dir})
       else ()
         message(FATAL_ERROR "Library lib${LIB} not found in ${OpenCASCADE_LIBRARY_DIR}")
       endif ()
     endif ()
   endforeach ()
 endmacro ()

 # Defines the occutils library the user should link with
 function (cxx_library_with_type name type)
   # type can be either STATIC or SHARED to denote a static or shared library.
   # ARGN refers to additional arguments after 'type'.
   add_library(${name} ${type} ${ARGN})
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
