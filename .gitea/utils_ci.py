"""Utils-specific CI policy."""

EXCLUDED_DIFF_PATHS = (
    "dependencies/",
    "docs/",
    "cmake/",
    "benchmarks/",
)

CI_REVIEW_CONTEXTS = (
    "clang-format",
    "clang-tidy",
    "unit-tests-gcc-debug",
    "unit-tests-gcc-release",
    "unit-tests-clang-debug",
    "unit-tests-clang-release",
    "valgrind",
)
