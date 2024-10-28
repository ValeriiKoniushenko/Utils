cmake_minimum_required(VERSION 3.30.0..)

project(Utils VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fixing unix clang lib dependencies
if(MSVC)
    # do nothing
else()
    # do nothing
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        message(STATUS "Clang: added flag[s]")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    else()
        # do nothing
    endif()
endif()

# Little crutch for GTests: if we use XSan -> settings 0 to HAVE_POSIX_REGEX
string(FIND ${CMAKE_CXX_FLAGS} "-fsanitize=" HAS_XSAN)
if(NOT ${HAS_XSAN} STREQUAL "")
    if(WIN32)
        set(HAVE_STD_REGEX 0)
    else()
        set(HAVE_POSIX_REGEX 0)
    endif()
endif()

set(DEPENDENCIES_DIR dependencies)

add_subdirectory(${DEPENDENCIES_DIR}/glm)
add_subdirectory(${DEPENDENCIES_DIR}/googletest)
add_subdirectory(${DEPENDENCIES_DIR}/benchmark)
add_subdirectory(benchmark)
add_subdirectory(tests)
add_subdirectory(sources)