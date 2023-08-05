cmake_minimum_required(VERSION 3.25)

option(OCCUTILS_BUILD_TESTS "Build tests" OFF)
if (OCCUTILS_BUILD_TESTS)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif ()

set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_CURRENT_LIST_DIR}/vcpkg/triplets" CACHE STRING "")

set(OCCUTILS_VERSION 0.0.0)
project(occutils VERSION ${OCCUTILS_VERSION} LANGUAGES CXX)

# Needed to set the namespace for both the export targets and the
# alias libraries
set(cmake_package_name occutils CACHE INTERNAL "")

# Set default build to release
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# make sure __cplusplus is defined when using msvc and enable parallel build
if (MSVC)
  string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
endif ()

if (NOT CYGWIN AND NOT MSYS AND NOT MINGW AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL QNX)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif ()

# ---------------------------------------------------------------------------------------
# Set OCCUTILS_MASTER_PROJECT to ON if we are building occutils
# ---------------------------------------------------------------------------------------
# Check if occutils is being used directly or via add_subdirectory, but allow overriding
if (NOT DEFINED OCCUTILS_MASTER_PROJECT)
  if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(OCCUTILS_MASTER_PROJECT ON)
  else ()
    set(OCCUTILS_MASTER_PROJECT OFF)
  endif ()
endif ()

# install options
option(OCCUTILS_INSTALL "Generate the install target" ${OCCUTILS_MASTER_PROJECT})

# These commands only run if this is the main project
if (OCCUTILS_MASTER_PROJECT)
  # BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
  # make it prominent in the GUI.
  option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)
endif ()

# warning options
option(OCCUTILS_BUILD_WARNINGS "Enable compiler warnings" OFF)

include(GNUInstallDirs)

# Define helper functions and macros used by occutils
include(cmake/internal_utils.cmake)

# Licensing
build_3rd_party_copyright()

# Dependencies
find_package(freetype CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(OpenCASCADE CONFIG REQUIRED)

# Build
cxx_library(${PROJECT_NAME} src/occutils-all.cc)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
                           ${OpenCASCADE_INCLUDE_DIR}
)

if (OCCUTILS_BUILD_WARNINGS)
  target_enable_warnings(${PROJECT_NAME})
endif ()

# If the CMake version supports it, attach header directory information
# to the targets for when we are part of a parent build (ie being pulled
# in via add_subdirectory() rather than being a standalone build).
if (DEFINED CMAKE_VERSION AND NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.11")
  target_include_directories(${PROJECT_NAME} PUBLIC
                             "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                             "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>"
  )
endif ()

# ---------------------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------------------
if (OCCUTILS_INSTALL)
  message(STATUS "Generating install")
  install(TARGETS ${PROJECT_NAME}
          EXPORT ${PROJECT_NAME}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY
          "${CMAKE_CURRENT_LIST_DIR}/include/"
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(FILES
          ${CMAKE_CURRENT_LIST_DIR}/README.md
          ${CMAKE_CURRENT_LIST_DIR}/LICENSE
          ${CMAKE_BINARY_DIR}/LICENSE-3RD-PARTY.txt
          DESTINATION ${CMAKE_INSTALL_DOCDIR})

  # Prepare package config and version files
  include(CMakePackageConfigHelpers)
  set(config_targets_file "${PROJECT_NAME}-targets.cmake")
  set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
  set(project_config_in "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in")
  set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
  set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")

  # Export target and install
  export(TARGETS ${PROJECT_NAME} NAMESPACE ${PROJECT_NAME}:: FILE "${CMAKE_CURRENT_BINARY_DIR}/${config_targets_file}")
  install(EXPORT ${PROJECT_NAME} DESTINATION ${export_dest_dir} NAMESPACE ${PROJECT_NAME}:: FILE ${config_targets_file})

  # Configure package config file
  configure_package_config_file("${project_config_in}" "${project_config_out}"
                                INSTALL_DESTINATION ${export_dest_dir})

  # Write version config file
  write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)

  # Install package config and version files
  install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

  # Support creation of installable packages
  include(cmake/occutils-cpack.cmake)
endif ()

# ---------------------------------------------------------------------------------------
# Testing
# ---------------------------------------------------------------------------------------
if (OCCUTILS_BUILD_TESTS)
  message(STATUS "Generating tests")
  enable_testing()
  add_subdirectory(test)
endif ()
