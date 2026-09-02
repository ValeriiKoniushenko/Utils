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
- `.gitea/`: repository CI entry points and reusable quality-check helpers.

The umbrella `Utils` target links `Utils::Core` and `Utils::Utils`.

## Build and test

Requirements are CMake 3.30+ and a C++23-capable compiler.

```sh
git submodule update --init --recursive
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/bin/UtilsTests
```

The standard test executable is `build/bin/UtilsTests`. Common configured build layouts are
`build/`, `build/clang/debug`, `build/clang/release`, `build/gcc/debug`, and `build/gcc/release`;
substitute the selected path consistently in configure, build, test, lint, and debug commands.

When Utils is the top-level project, CMake automatically uses `ccache` when it is available and no
compiler launcher was provided explicitly. Use `-DUTILS_DISABLE_TESTS=ON` or
`-DUTILS_DISABLE_BENCHMARKS=ON` to omit those targets. These are disable switches. Prefer an
incremental build; create a clean build only for stale state, structural CMake changes, or compiler
switches.

For a clean checkout, synchronize submodule URLs and initialize the commits recorded by the
repository before configuring:

```sh
git submodule sync --recursive
git submodule update --init --recursive
```

Do not use `--remote` or `--force` for the normal clean-build workflow.

## Benchmark targets

Configure benchmarks in Release mode, then build and run these executables:

```sh
cmake -S . -B build/benchmarks -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DUTILS_DISABLE_BENCHMARKS=OFF \
  -DUTILS_DISABLE_TESTS=ON
cmake --build build/benchmarks --parallel
build/benchmarks/bin/UtilsBench_Math
build/benchmarks/bin/UtilsBench_String
build/benchmarks/bin/UtilsBench_ProdStringsCase
```

- `UtilsBench_Math`: arithmetic and math-helper measurements.
- `UtilsBench_String`: general string-operation measurements.
- `UtilsBench_ProdStringsCase`: production-style string and navigation workloads.

With the default `build/` configuration, these executables are located at
`build/bin/UtilsBench_Math`, `build/bin/UtilsBench_String`, and
`build/bin/UtilsBench_ProdStringsCase`. The isolated Release configuration above places them
under `build/benchmarks/bin/` instead.

List registered benchmarks with `--benchmark_list_tests`. Focus a run with
`--benchmark_filter='<regular-expression>'`. For comparisons, keep the machine, build
configuration, benchmark arguments, and power mode constant; use repeated aggregate samples.

## Test target

Unit-test sources live under `tests/` and build into the `UtilsTests` executable. Build it with
the selected CMake build directory and run the resulting executable from its `bin/` directory:

```sh
cmake --build build --parallel --target UtilsTests
./build/bin/UtilsTests
```

Run the complete suite before reporting a behavior change as verified. Use GoogleTest filtering
only for focused iteration, and report any skipped or unavailable tests explicitly.

Use all available CPU threads for builds, tests, and other parallelizable analysis or validation work,
unless a tool or resource constraint requires limiting parallelism.

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

At the end of every documentation-generation task, after all documentation and navigation edits,
run this exact Antora build command:

```sh
cd docs
npx antora --fetch antora-playbook.yml
```

Keep `README.md` limited to the project description, quick start, and documentation link.

## Local quality checks

The `.gitea/` CI helpers define the repository's clang-format, clang-tidy, and Valgrind checks.
For local work, format changed C++ files, run clang-tidy against the active build's
`compile_commands.json`, and run the test executable under Valgrind when a runtime memory check is
needed:

```sh
clang-format --dry-run --Werror path/to/changed-file.cpp
run-clang-tidy -p build -j 16
valgrind --leak-check=full --show-leak-kinds=definite \
  --track-origins=yes build/bin/UtilsTests
```
