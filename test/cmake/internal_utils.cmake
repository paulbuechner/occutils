# Configure OCCT libraries to link with
macro (target_link_occt_libraries target)
  # Set debug libraries on debug mode (WIN32 only)
  if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(DEBUG_POSTFIX "d")
  endif ()

  foreach (LIB ${OpenCASCADE_LIBRARIES})
    if (WIN32)
      if (EXISTS "${OpenCASCADE_LIBRARY_DIR}${DEBUG_POSTFIX}/${LIB}.lib")
        set(occt_lib_dir "${OpenCASCADE_LIBRARY_DIR}${DEBUG_POSTFIX}/${LIB}.lib")
        target_link_libraries(${target} debug ${occt_lib_dir})
        target_link_libraries(${target} optimized ${occt_lib_dir})
      else ()
        message(FATAL_ERROR "Library ${LIB} not found in ${OpenCASCADE_LIBRARY_DIR}${DEBUG_POSTFIX}")
      endif ()
    elseif (APPLE)
      if (EXISTS ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.dylib)
        set(occt_lib_dir ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.dylib)
        target_link_libraries(${target} debug ${occt_lib_dir})
        target_link_libraries(${target} optimized ${occt_lib_dir})
      else ()
        message(FATAL_ERROR "Library lib${LIB} not found in ${OpenCASCADE_LIBRARY_DIR}")
      endif ()
    elseif (UNIX)
      if (EXISTS ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so)
        set(occt_lib_dir ${OpenCASCADE_LIBRARY_DIR}/lib${LIB}.so)
        target_link_libraries(${target} debug ${occt_lib_dir})
        target_link_libraries(${target} optimized ${occt_lib_dir})
      else ()
        message(FATAL_ERROR "Library lib${LIB} not found in ${OpenCASCADE_LIBRARY_DIR}")
      endif ()
    endif ()
  endforeach ()
endmacro ()

# Configure OCCT libraries to link with (vcpkg)
macro (target_link_occt_libraries_vcpkg target)
  foreach (LIB ${OpenCASCADE_LIBRARIES})
    target_link_libraries(${target} debug ${LIB})
    target_link_libraries(${target} optimized ${LIB})
  endforeach ()
endmacro ()
