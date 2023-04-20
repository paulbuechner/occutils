cmake_minimum_required(VERSION 3.24)

option(OCCUTILS_BUILD_TESTS "Build tests" OFF)
if (OCCUTILS_BUILD_TESTS)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif ()

set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_CURRENT_LIST_DIR}/vcpkg/triplets" CACHE STRING "")

set(OCCUtils_VERSION 0.1.0)
project(occutils VERSION ${OCCUtils_VERSION} LANGUAGES CXX)

# Needed to set the namespace for both the export targets and the
# alias libraries
set(cmake_package_name occutils CACHE INTERNAL "")

# Set default build to release
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

# Compiler config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# make sure __cplusplus is defined when using msvc and enable parallel build
if (MSVC)
  string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
endif ()

set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SYSTEM_NAME MATCHES "CYGWIN" OR CMAKE_SYSTEM_NAME MATCHES "MSYS")
  set(CMAKE_CXX_EXTENSIONS ON)
endif ()

# These commands only run if this is the main project
if (CMAKE_PROJECT_NAME STREQUAL "occutils")
  # BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
  # make it prominent in the GUI.
  option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)
endif ()

# warning options
option(OCCUTILS_BUILD_WARNINGS "Enable compiler warnings" OFF)

# Define helper functions and macros used by occutils
include(cmake/internal_utils.cmake)

include(GNUInstallDirs)

find_package(OpenCASCADE CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED) # locate freetype when using vcpkg
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(GTest CONFIG REQUIRED)

cxx_library(${PROJECT_NAME} src/occutils-all.cc)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
                           ${OpenCASCADE_INCLUDE_DIR}
                           ${Boost_INCLUDE_DIRS}
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

# Configure install
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY
        "${CMAKE_CURRENT_LIST_DIR}/include/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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

# Testing
if (OCCUTILS_BUILD_TESTS)
  message(STATUS "Generating tests")
  enable_testing()
  add_subdirectory(test)
endif ()
