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
- `.agents/skills/`: generalized agent workflows linked from the shared Agents submodule.
- `.agents/Agents/`: shared submodule that owns those workflows; do not modify it or edit through
  the skill links unless the user explicitly requests shared-skill work.

The umbrella `Utils` target links `Utils::Core` and `Utils::Utils`.

## Agent skill configuration

The workflows under `.agents/skills/` are intentionally repository-independent. This file is the
source of truth for every Utils-specific target, executable, option, directory, prerequisite, and
pass criterion they need. The current skills map to the following sections:

- `build` and `clean-build`: [Build and clean build](#build-and-clean-build).
- `test` and `verification-run`: [Tests and verification](#tests-and-verification).
- `benchmark` and `ecs-benchmarks`: [Benchmarks](#benchmarks).
- `code-coverage`: [Code coverage](#code-coverage).
- `codegen`: [Code generation](#code-generation).
- `docs-generation`: [Documentation](#documentation).
- `lint-and-format`: [Formatting and static analysis](#formatting-and-static-analysis).
- `valgrind`: [Memory checks](#memory-checks).

When a workflow is unsupported, its section says so explicitly. Do not infer a target, generator,
or executable from a similarly named facility in another repository.

## Build and clean build

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
Executables are emitted to `<build_dir>/bin/` and libraries to `<build_dir>/lib/`.

When Utils is the top-level project, CMake automatically uses `ccache` when it is available and no
compiler launcher was provided explicitly. Use `-DUTILS_DISABLE_TESTS=ON` or
`-DUTILS_DISABLE_BENCHMARKS=ON` to omit those targets. These are disable switches. Prefer an
incremental build; create a clean build only for stale state, structural CMake changes, or compiler
switches.

The `build` skill always reuses an existing configured directory and runs `cmake --build
<build_dir> --parallel`. Configure only when that directory does not exist. Use `clean-build` only
for stale or inconsistent output, structural CMake changes, or compiler switches. Delete only the
selected build directory; never delete the repository root or any source, test, benchmark,
documentation, dependency, or agent directory.

For a clean checkout or clean rebuild of the default `build/` directory, synchronize submodule URLs
and initialize the commits recorded by the repository before configuring:

```sh
git submodule sync --recursive
git submodule update --init --recursive
rm -rf build
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

Do not use `--remote` or `--force` for the normal clean-build workflow.

## Benchmarks

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

There is no ECS subsystem, ECS benchmark source, ECS benchmark target, or ECS-specific harness in
this repository. The `ecs-benchmarks` skill is therefore unsupported: report it as unavailable and
do not substitute one of the three general benchmark executables. There are no ECS code-generation
or coverage prerequisites.

## Tests and verification

Unit-test sources live under `tests/` and build into the `UtilsTests` executable. Build it with
the selected CMake build directory and run the resulting executable from its `bin/` directory:

```sh
cmake --build build --parallel --target UtilsTests
./build/bin/UtilsTests
```

Run the complete suite before reporting a behavior change as verified. Use GoogleTest filtering
only for focused iteration, and report any skipped or unavailable tests explicitly.

The project does not call `enable_testing()` or register `add_test()` entries, so do not use CTest
as the primary test runner. The CI-aligned local wrapper for the default build is:

```sh
python3 .gitea/check_unit_tests.py \
  --executable build/bin/UtilsTests \
  --label "Local Debug" \
  --context unit-tests-local \
  --no-gitea \
  --verbose
```

For the `verification-run` skill, build the selected directory first, then run its complete
`<build_dir>/bin/UtilsTests` suite with no filter. There is no separate runtime smoke-check
executable, required argument set, or project-specific local timeout. A verification passes only
when the build and the full test executable both exit zero; CI gives each unit-test job 10 minutes.
Documentation-only verification is defined separately under [Documentation](#documentation).

Use all available CPU threads for builds, tests, and other parallelizable analysis or validation
work, unless a tool or resource constraint requires limiting parallelism.

## Code coverage

Coverage uses Clang source-based instrumentation in a dedicated Debug build. The only project switch
is `BUILD_UTILS_WITH_TEST_COVERAGE=ON`; it has no effect for GCC, MSVC, or non-Debug builds. Tests
must remain enabled, and benchmarks are not part of the coverage run. The repository currently has
no CMake coverage target, no coverage threshold, and no committed report-generator wrapper.

The local `code-coverage` workflow requires `clang++` plus matching `llvm-profdata` and `llvm-cov`
binaries:

```sh
cmake -S . -B build/coverage -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DUTILS_DISABLE_TESTS=OFF \
  -DUTILS_DISABLE_BENCHMARKS=ON \
  -DBUILD_UTILS_WITH_TEST_COVERAGE=ON
cmake --build build/coverage --parallel --target UtilsTests
cmake -E make_directory build/coverage/profiles build/coverage/coverage-html
LLVM_PROFILE_FILE='build/coverage/profiles/UtilsTests-%p-%m.profraw' \
  build/coverage/bin/UtilsTests
llvm-profdata merge -sparse build/coverage/profiles/*.profraw \
  -o build/coverage/UtilsTests.profdata
llvm-cov show build/coverage/bin/UtilsTests \
  -instr-profile=build/coverage/UtilsTests.profdata \
  -format=html \
  -output-dir=build/coverage/coverage-html \
  -ignore-filename-regex='(.*/dependencies/.*|.*/tests/.*)'
llvm-cov report build/coverage/bin/UtilsTests \
  -instr-profile=build/coverage/UtilsTests.profdata \
  -ignore-filename-regex='(.*/dependencies/.*|.*/tests/.*)'
test -f build/coverage/coverage-html/index.html
```

The report is `build/coverage/coverage-html/index.html`; raw profiles and the merged profile stay
under `build/coverage/`. Do not run concurrent coverage jobs in that directory. Treat a missing LLVM
tool, test failure, merge failure, report failure, or absent `index.html` as a failed/unavailable
coverage run. `.gitea/publish_code_coverage.py` only publishes an already generated gcovr JSON
summary and single-file HTML report in CI; it is not the local report generator above.

## Code generation

Utils currently has no source-code generator, metadata generator, generator cache/state file,
generated-output directory, CMake generation target, or regeneration trigger. In particular,
`sources/Core/Enum.h` implements enum helpers with compile-time macros; the "generated enums"
documentation does not describe generated source files. The `codegen` skill is therefore
unsupported. Do not edit purported generated files or invent a regeneration command; report that
no project code-generation workflow is configured. If one is added later, document its trigger,
command, state files, outputs, and required follow-up build here before using the skill.

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

## Formatting and static analysis

The `lint-and-format` skill uses the repository-root `.clang-format` and `.clang-tidy` files. Scope
the normal check to changed C++ files under `sources/`, `tests/`, and `benchmarks/`; use a full-tree
check only when explicitly requested. The CI-aligned local helpers compare against `develop`, avoid
external review/status writes with `--no-gitea`, and accept `--files <paths...>` instead of a Git
diff when an explicit file list is more appropriate:

```sh
python3 .gitea/check_clang_format.py \
  --base develop \
  --no-gitea \
  --verbose
python3 .gitea/check_clang_tidy.py \
  --base develop \
  --build-dir build \
  --jobs "$(nproc)" \
  --no-gitea \
  --verbose
```

Pass the selected build directory consistently to `--build-dir`; it must contain
`compile_commands.json`. Use `--fix` with `check_clang_format.py` to apply formatting, then review
the diff. For a single changed file, `clang-format --dry-run --Werror <path>` is also acceptable.
Always build the affected target after applying formatter or tidy fixes.

## Memory checks

The `valgrind` skill uses the Clang Debug `UtilsTests` executable and the repository wrapper. Build
the full test target with debug information first, then run:

```sh
python3 .gitea/check_valgrind.py \
  --executable build/clang/debug/bin/UtilsTests \
  --no-gitea \
  --verbose
```

Substitute another selected Clang Debug build directory consistently. The wrapper applies
`--leak-check=full`, `--show-leak-kinds=definite`, `--errors-for-leak-kinds=definite`,
`--track-origins=yes`, and error exit code 42. No project suppression file is configured. Treat
invalid accesses, uninitialized reads, any nonzero Valgrind error summary, definite leaks, or unit
test failures as failures. GoogleTest filters may be passed after `--` for focused diagnosis, but
run the unfiltered suite before reporting the memory check as verified. CI allows the Valgrind job
20 minutes; report a missing Valgrind binary or startup failure as unavailable rather than passing.
