cmake_minimum_required(VERSION 3.24)

set(OCCUtils_VERSION 0.1.0)
project(occutils VERSION ${OCCUtils_VERSION} LANGUAGES CXX)

# Define helper functions and macros used by occutils
include(cmake/internal_utils.cmake)

enable_testing()

include(GNUInstallDirs)

# Set default build to release
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

# Check if vcpkg is being used
set(VCPKG_IS_USED OFF)
if (CMAKE_TOOLCHAIN_FILE MATCHES ".*/vcpkg/scripts/buildsystems/vcpkg.cmake$")
  set(VCPKG_IS_USED ON)
  # Check if should use local install of OpenCascade
  set(USE_LOCAL_OCC OFF CACHE BOOL "Use local OpenCascade install")
  if (CUSTOM_OpenCASCADE_DIR)
    message(STATUS "Use local OpenCascade install: ${CUSTOM_OpenCASCADE_DIR}")
    set(OpenCASCADE_DIR ${CUSTOM_OpenCASCADE_DIR})
    set(USE_LOCAL_OCC ON)
    # Do not copy dependencies to the output directory
    set(VCPKG_APPLOCAL_DEPS OFF)
  endif ()
endif ()

# Dependencies
if (VCPKG_IS_USED)
  if (USE_LOCAL_OCC)
    find_package(OpenCASCADE REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
  else ()
    find_package(OpenCASCADE CONFIG REQUIRED)
    find_package(freetype CONFIG REQUIRED) # locate freetype when using vcpkg
  endif ()
else ()
  find_package(OpenCASCADE REQUIRED)
endif ()

find_package(Boost REQUIRED COMPONENTS system filesystem unit_test_framework)

# Where occutils's .h files can be found
set(occutils_include_dirs
    "${OpenCASCADE_INCLUDE_DIR}"
    "${Boost_INCLUDE_DIRS}"
    "${occutils_SOURCE_DIR}/include"
    "${occutils_SOURCE_DIR}")
include_directories(SYSTEM ${occutils_include_dirs})

set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SYSTEM_NAME MATCHES "CYGWIN" OR CMAKE_SYSTEM_NAME MATCHES "MSYS")
  set(CMAKE_CXX_EXTENSIONS ON)
endif ()

cxx_library(${PROJECT_NAME} src/occutils-all.cc)
# If the CMake version supports it, attach header directory information
# to the targets for when we are part of a parent build (ie being pulled
# in via add_subdirectory() rather than being a standalone build).
if (DEFINED CMAKE_VERSION AND NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.11")
  string(REPLACE ";" "$<SEMICOLON>" dirs "${occutils_include_dirs}")
  target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE "$<BUILD_INTERFACE:${dirs}>"
                             "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
endif ()

if (MSVC)
  # warning level 4 and all warnings as errors
  target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
else ()
  # lots of warnings and all warnings as errors
  target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic -Werror)
endif ()
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Configure install
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY
        "${CMAKE_CURRENT_LIST_DIR}/include/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Testing
if (${occutils_build_tests})
  add_subdirectory(test)
endif ()
