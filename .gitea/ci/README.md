# Shared C++ CI helpers

This package is portable between repositories. It provides:

- `gitea_client.py`: Gitea reviews, commit statuses, and attachments;
- `diff.py`: changed C/C++ file discovery and analysis-target selection;
- `clang_format.py` and `clang_tidy.py`: configurable checker implementations;
- `coverage.py`: gcovr-summary PR publishing and ZIP report attachment;
- `valgrind.py`: Valgrind test execution and PR reporting;
- `cleanup.py`: replacement-review and attachment cleanup.

Keep repository policy outside this directory. A consuming repository needs a
small configuration module for excluded paths and review contexts, then thin
entry points such as `main(excluded_paths=PROJECT_EXCLUDED_PATHS)`.
