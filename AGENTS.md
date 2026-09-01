# Utils Project Guide

## Purpose

Utils is a cross-platform C++23 utility library built with CMake. It provides reusable helpers for
geometry, colors, math, strings and regular expressions, intrusive ownership, delegates, enums,
timers, assertions, files, and CSV data.

## Repository map

- `sources/Core/`: higher-level utilities exposed by `Utils::Core`.
- `sources/Utils/`: concepts, traits, file/CSV helpers, and low-level utilities exposed by
  `Utils::Utils`.
- `tests/`: GoogleTest sources for the `UtilsTests` executable.
- `benchmarks/`: Google Benchmark targets for math and strings.
- `docs/`: Antora/AsciiDoc documentation sources and validation tooling.
- `cmake/`: shared compiler-feature and warning configuration.
- `dependencies/`: third-party submodules; do not inspect or edit unless explicitly requested.
- `docs/html/`: generated documentation; never use it as a source or edit it manually.

The umbrella `Utils` target links `Utils::Core` and `Utils::Utils`.

## Build and test

Requirements are CMake 3.30+ and a C++23-capable compiler.

```sh
git submodule update --init --recursive
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/bin/UtilsTests
```

Use `-DUTILS_DISABLE_TESTS=ON` or `-DUTILS_DISABLE_BENCHMARKS=ON` to omit those targets. Prefer an
incremental build; create a clean build only for stale state, structural CMake changes, or compiler
switches.

## Change guidelines

- Preserve unrelated user changes and keep edits focused.
- Add or update tests for behavior changes.
- Run clang-format on changed C++ lines/files and build the affected targets.
- Use Release builds for benchmarks and compare repeated samples on the same machine.
- Public headers are included as `Core/...` or `Utils/...`; verify names against the source tree.

## Documentation

AsciiDoc pages live in `docs/modules/ROOT/pages/`; navigation lives in
`docs/modules/ROOT/nav.adoc`. Add or rename a page and its navigation entry together.

Real code samples must be tagged in source/tests and included through Antora `include::` directives,
not copied into pages. Validate documentation with:

```sh
cd docs
python3 scripts/validate_docs.py
npm install
npm run docs
```

Keep `README.md` limited to the project description, quick start, and documentation link.
