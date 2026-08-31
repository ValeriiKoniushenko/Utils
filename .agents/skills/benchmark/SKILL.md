---
name: benchmark
description: Build and run Utils Google Benchmark performance measurements when changing or investigating StringHelper or FileNavigator behavior.
---

# Benchmark

Use this skill for performance-sensitive changes to `String.h`, `Delegate.h`, or
`IntrusivePtr.h`, and when a user asks for benchmark results or a
performance-regression check. Do not use it for ordinary unit-test or compilation-only work.

Benchmark sources and their CMake target are in the project-root `benchmarks/` directory. Google
Benchmark is the `dependencies/benchmark` submodule, pinned by the superproject.

## Prepare a comparable run

1. Configure a Release build with benchmarks enabled. Do not use Debug results for
   performance conclusions.

   ```sh
   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DJRM_ENABLE_BENCHMARKS=ON
   ```

2. Build incrementally using the `build` skill:

   ```sh
   cmake --build build --parallel
   ```

For before/after comparisons, use the same machine, build directory configuration, and
benchmark options. Close CPU-intensive applications and avoid comparing measurements from
different machines or power modes.

## Run and inspect results

Run all registered benchmarks:

```sh
build/bin/UtilsBench_Math
build/bin/UtilsBench_String
build/bin/UtilsBench_ProdStringsCase
```

List available benchmarks without measuring them:

```sh
build/bin/UtilsBench_ProdStringsCase --benchmark_list_tests
```

Run a focused group while iterating, for example navigation benchmarks:

```sh
build/bin/UtilsBench_ProdStringsCase --benchmark_filter='BM_(GoToLineStart|FindFirstWithLineLimit|GetLineNumberAndColumn)'
```

For a comparison-quality report, take repeated samples and write JSON output outside the
source tree or in an ignored build directory:

```sh
build/bin/UtilsBench_ProdStringsCase \
  --benchmark_repetitions=10 \
  --benchmark_report_aggregates_only=true \
  --benchmark_format=json \
  --benchmark_out=build/benchmark-results.json
```

Compare the aggregate `real_time` values for the same benchmark names and argument sets.
Treat small differences as noise unless repetitions show a consistent change; report the
command, build type, and whether the result is CPU-time or real-time based.
