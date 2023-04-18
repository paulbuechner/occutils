cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(occutils-test VERSION ${OCCUtils_VERSION} LANGUAGES CXX)

# Define helper functions and macros used by occutils
include(cmake/internal_utils.cmake)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Compile test suite
add_executable(${PROJECT_NAME} TestLine.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "occutils/include")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(boost_test_libs
    Boost::boost
    Boost::unit_test_framework
    Boost::system
    Boost::filesystem)
target_link_libraries(${PROJECT_NAME} debug occutils ${boost_test_libs})
target_link_libraries(${PROJECT_NAME} optimized occutils ${boost_test_libs})

if (VCPKG_IS_USED AND NOT USE_LOCAL_OCC)
  target_link_occt_libraries_vcpkg(${PROJECT_NAME})
else ()
  target_link_occt_libraries(${PROJECT_NAME})
endif ()

# Configure test suite
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
