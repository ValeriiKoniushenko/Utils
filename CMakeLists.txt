cmake_minimum_required(VERSION 3.30.0..)

project(Utils VERSION 1.2.2 LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Predefinitions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Features.cmake)

if (MSVC)
    add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else ()
    add_compile_options(
        "-Wall"
        "-Wextra"
        "-Werror"
        "-Wno-error=unused-variable"
        "-Wno-unused-variable"
        "$<$<CONFIG:DEBUG>:-g3>"
        "$<$<CONFIG:DEBUG>:-O0>"
        "$<$<CONFIG:DEBUG>:-fno-inline>"
        "$<$<CONFIG:DEBUG>:-fno-omit-frame-pointer>"
        "$<$<CONFIG:RELEASE>:-O3>"
    )
endif ()


# ========= Base setup & Predefinitions =========
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
