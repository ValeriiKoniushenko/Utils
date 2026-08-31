cmake_minimum_required(VERSION 3.30.0..)

project(Utils VERSION 1.3.2 LANGUAGES CXX)

cmake_policy(SET CMP0141 NEW)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>"
)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    if (NOT CMAKE_CXX_COMPILER_LAUNCHER)
        find_program(CCACHE_PROGRAM ccache)
        if (CCACHE_PROGRAM)
            set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
            set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
            message(STATUS "ccache: ${CCACHE_PROGRAM}")
        endif ()
    endif ()
endif ()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Features.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Flags.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ========= Base setup & Predefinitions =========
set(BUILD_UTILS_WITH_TEST_COVERAGE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(REQUIRED_CXX_STANDARD cxx_std_23) # read-only
set(CXX_STANDARD)

Utils_InstallCXXStandardVersion()

# ========= Adding subdirectories =========
add_subdirectory(dependencies)

if (NOT UTILS_DISABLE_BENCHMARKS)
    add_subdirectory(benchmarks)

    # benchmark target is Google Benchmark
    target_compile_options(benchmark PUBLIC $<$<CXX_COMPILER_ID:Clang,GNU>:-Wno-c2y-extensions>)
endif ()

if (NOT UTILS_DISABLE_TESTS)
    add_subdirectory(tests)
endif ()

add_subdirectory(sources)