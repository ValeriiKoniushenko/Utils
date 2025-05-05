cmake_minimum_required(VERSION 3.30.0..)

project(Utils VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(Predefinitions)
include(Features)

# ========= Base setup & Predefinitions =========
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(REQUIRED_CXX_STANDARD cxx_std_23) # read-only
set(CXX_STANDARD)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(DEPENDENCIES_DIR dependencies)
set(BENCHMARK_ENABLE_TESTING)

InstallCXXStandardVersion()
InstallDebugLevel()
InstallBenchmarkDependenciesTesting()


# ========= Adding subdirectories =========
# Adding dependencies
add_subdirectory(${DEPENDENCIES_DIR}/glm)
add_subdirectory(${DEPENDENCIES_DIR}/googletest)
add_subdirectory(${DEPENDENCIES_DIR}/benchmark)
add_subdirectory(${DEPENDENCIES_DIR}/pcre2)

# Adding core
add_subdirectory(benchmark)
add_subdirectory(tests)
add_subdirectory(sources)
