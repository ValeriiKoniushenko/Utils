cmake_minimum_required(VERSION 3.30.0..)

project(Utils VERSION 1.2.2 LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Predefinitions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Features.cmake)

# ========= Base setup & Predefinitions =========
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(REQUIRED_CXX_STANDARD cxx_std_23) # read-only
set(CXX_STANDARD)

set(DEPENDENCIES_DIR dependencies)
set(BENCHMARK_ENABLE_TESTING)

Utils_InstallCXXStandardVersion()
Utils_InstallDebugLevel()
Utils_InstallBenchmarkDependenciesTesting()


# ========= Adding subdirectories =========
# Adding dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchDependencies.cmake)

# Adding core
add_subdirectory(benchmark)
add_subdirectory(tests)
add_subdirectory(sources)
