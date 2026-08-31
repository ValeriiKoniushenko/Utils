# Utils Project Guide

## Project description

Utils is a cross-platform C++23 utility library built with CMake. It provides reusable
types and helpers for geometry, math, strings, ownership, events, enums, and assertions.

## Overview

- `sources/Core/` contains the main higher-level utilities; link with `Utils::Core`.
- `sources/Utils/` contains concepts, traits, general helpers, and CSV/string utilities;
  link with `Utils::Utils`.
- The umbrella `Utils` target links both libraries.
- `tests/` contains the GoogleTest executable `UtilsTests`.
- `benchmarks/` contains Google Benchmark targets for strings and math.
- `dependencies/` holds third-party dependencies managed by the root CMake project.
- CMake 3.30+ and a C++23-capable compiler are required (README recommends Clang
  18.1.8+ or GCC 14.2.1+). Ccache is detected automatically when available.

## Main modules

- Concepts, type traits, utility functions, cross-platform strings, and EasyCSV.
- Geometry and value types: `Rect`, `Position`, `Size`, and `Color`.
- Compile-time math helpers, timers, singleton helpers, and abstract iterators.
- Intrusive strong/weak pointers with optional lifetime-safe weak loading.
- Compile-time enum generation and string conversion.
- Atomic/dynamic strings, string helpers, and PCRE2-backed regular expressions.
- Delegates/subscribers for event-oriented callbacks.
- Runtime `Assert` and `Verify` diagnostics with optional stack traces.

## Build and run

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/UtilsTests
./build/bin/UtilsBench_String  # optional benchmark
```
