cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(occutils-test VERSION ${OCCUTILS_VERSION} LANGUAGES CXX)

# Define helper functions and macros used by occutils
include(cmake/occutils_internal_utils.cmake)

find_package(GTest CONFIG REQUIRED)

# Set libs to link against
list(APPEND occutils_test_libs
     occutils::occutils
     GTest::gtest
     GTest::gtest_main
     ${OpenCASCADE_LIBRARIES}
)

# Add executable
occutils_test_cxx_executable(${PROJECT_NAME} src/occutils-test-all.cc "${occutils_test_libs}")

# Add target_include_directories
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
                           ${OpenCASCADE_INCLUDE_DIR}
)

add_test(NAME ${PROJECT_NAME}
         COMMAND ${PROJECT_NAME}
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
