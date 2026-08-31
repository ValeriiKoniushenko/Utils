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
    "valgrind",
)
