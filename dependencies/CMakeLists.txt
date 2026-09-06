
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)


# ── Benchmark ───────────────────────────────────────────────────────────────────────────────
if(NOT UTILS_DISABLE_BENCHMARKS)
    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory(benchmark)
endif()

# ── GLM ───────────────────────────────────────────────────────────────────────────────
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(glm)

# ── GoogleTest ───────────────────────────────────────────────────────────────────────────────
if(NOT UTILS_DISABLE_TESTS)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(googletest)
endif()

# ── PCRE2 ───────────────────────────────────────────────────────────────────────────────
set(PCRE2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(PCRE2_BUILD_PCRE2GREP OFF CACHE BOOL "" FORCE)
add_subdirectory(pcre2)


set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)
