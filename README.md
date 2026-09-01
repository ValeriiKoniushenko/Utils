# Utils

Utils is a cross-platform C++23 utility library built with CMake. It provides reusable types and
helpers for geometry, math, strings, ownership, events, enums, assertions, files, and CSV data.

## Quick start

Requirements: CMake 3.30+, a C++23-capable compiler, Git, and the repository submodules.

```sh
git submodule update --init --recursive
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/bin/UtilsTests
```

Link `Utils` for the entire library, `Utils::Core` for higher-level facilities, or `Utils::Utils`
for the foundational concepts, traits, file, and CSV helpers.

## Documentation

The complete, structured AsciiDoc documentation starts at
[docs/modules/ROOT/pages/index.adoc](docs/modules/ROOT/pages/index.adoc). It includes installation,
CMake integration, library guides, contributor workflows, security policy, and the code of conduct.
