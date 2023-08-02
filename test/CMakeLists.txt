cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(occutils-test VERSION ${OCCUTILS_VERSION} LANGUAGES CXX)

# Define helper functions and macros used by occutils
include(cmake/internal_utils.cmake)

# Set libs to link against
list(APPEND occutils_test_libs
     occutils::occutils
     GTest::gtest
     GTest::gtest_main
     ${OpenCASCADE_LIBRARIES}
)

# Add executable
cxx_executable("occutils-test-all" src "${occutils_test_libs}")

# Add target_include_directories
target_include_directories("occutils-test-all" SYSTEM PRIVATE
                           ${OpenCASCADE_INCLUDE_DIR}
)

add_test(NAME ${PROJECT_NAME} COMMAND "occutils-test-all")
