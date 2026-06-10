
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)


# ── Benchmark ───────────────────────────────────────────────────────────────────────────────
set(BENCHMARK_ENABLE_TESTING OFF)
add_submodule(benchmark)

# ── GLM ───────────────────────────────────────────────────────────────────────────────
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_submodule(glm)

# ── GoogleTest ───────────────────────────────────────────────────────────────────────────────
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_submodule(googletest)

# ── PCRE2 ───────────────────────────────────────────────────────────────────────────────
set(PCRE2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(PCRE2_BUILD_PCRE2GREP OFF CACHE BOOL "" FORCE)
add_submodule(pcre2)


set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)
