cmake_minimum_required(VERSION 3.30.0..)

project(Utils VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(DEPENDENCIES_DIR dependencies)


# Little crutch for GTests: if we use XSan -> settings 0 to HAVE_POSIX_REGEX
if(NOT ${CMAKE_CXX_FLAGS} STREQUAL "")
    string(FIND ${CMAKE_CXX_FLAGS} "-fsanitize=" HAS_XSAN)
    if(NOT ${HAS_XSAN} STREQUAL "")
        if(WIN32)
            set(HAVE_STD_REGEX 0)
        else()
            set(HAVE_POSIX_REGEX 0)
        endif()
    endif()
endif()


# ========= Detecting build type dependencies =========
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG=1)
else()
    add_compile_definitions(DEBUG=0)
endif()


# ========= Adding dependencies =========
add_subdirectory(${DEPENDENCIES_DIR}/glm)

add_subdirectory(${DEPENDENCIES_DIR}/googletest)

set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(${DEPENDENCIES_DIR}/benchmark)


# ========= Adding core =========
add_subdirectory(benchmark)
add_subdirectory(tests)
add_subdirectory(sources)