option(occutils_build_tests "Build all of occutils's own tests." OFF)

cmake_minimum_required(VERSION 3.24)

project(occutils)

# Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system filesystem unit_test_framework)

# Set OCCT cmake directory on windows
if (WIN32)
  set(OpenCASCADE_DIR "C:/Program Files/OCCT/cmake")
endif ()
find_package(OpenCASCADE REQUIRED)

# Where liboccutils's .h files can be found
set(occutils_include_dirs
    "${Boost_INCLUDE_DIRS}"
    "${OpenCASCADE_INCLUDE_DIR}"
    "${occutils_SOURCE_DIR}/include"
    "${occutils_SOURCE_DIR}")
include_directories(SYSTEM ${occutils_include_dirs})

# Define helper functions and macros used by liboccutils
include(cmake/internal_utils.cmake)

set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SYSTEM_NAME MATCHES "CYGWIN" OR CMAKE_SYSTEM_NAME MATCHES "MSYS")
  set(CMAKE_CXX_EXTENSIONS ON)
endif ()

cxx_library(${PROJECT_NAME} src/occutils-all.cc)

if (MSVC)
  # warning level 4 and all warnings as errors
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else ()
  # lots of warnings and all warnings as errors
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif ()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/include")

#set_target_properties(${PROJECT_NAME}
#                      PROPERTIES
#                      INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include"
#                      INTERFACE_LINK_LIBRARIES "${PROJECT_NAME}")

target_link_occt_libraries(${PROJECT_NAME})

# Configure install
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include)
install(DIRECTORY
        "${CMAKE_CURRENT_LIST_DIR}/include/"
        DESTINATION include)

# Testing
if (${occutils_build_tests})
  enable_testing()
  # Compile test suite
  add_executable(test_occutils tests/TestLine.cpp)

  target_include_directories(test_occutils PUBLIC "${PROJECT_NAME}/include")
  target_compile_features(test_occutils PRIVATE cxx_std_17)

  set(boost_test_libs
      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
      ${Boost_FILESYSTEM_LIBRARY}
      ${Boost_SYSTEM_LIBRARY})
  target_link_libraries(test_occutils ${PROJECT_NAME} ${boost_test_libs})

  # Configure test suite
  add_test(NAME test_occutils COMMAND test_occutils)
endif ()
