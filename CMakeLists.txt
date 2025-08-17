cmake_minimum_required(VERSION 3.30.0..)

project(Utils VERSION 1.3.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Predefinitions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Features.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Flags.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ========= Base setup & Predefinitions =========
set(BUILD_UTILS_WITH_TEST_COVERAGE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(REQUIRED_CXX_STANDARD cxx_std_23) # read-only
set(CXX_STANDARD)

set(DEPENDENCIES_DIR dependencies)
set(BENCHMARK_ENABLE_TESTING)

Utils_InstallCXXStandardVersion()
Utils_InstallBenchmarkDependenciesTesting()

# ========= Adding subdirectories =========
# Adding dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchDependencies.cmake)

# Adding core
add_subdirectory(benchmark)
add_subdirectory(tests)
add_subdirectory(sources)

Utils_InstallDebugLevel()
