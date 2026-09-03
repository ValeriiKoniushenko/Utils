# tag::incremental_build[]
cmake --build build --parallel
./build/bin/UtilsTests
# end::incremental_build[]

# tag::clean_build[]
git submodule sync --recursive
git submodule update --init --recursive
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
# end::clean_build[]

# tag::release_benchmarks[]
cmake -S . -B build/benchmarks -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DUTILS_DISABLE_BENCHMARKS=OFF \
  -DUTILS_DISABLE_TESTS=ON
cmake --build build/benchmarks --parallel
build/benchmarks/bin/UtilsBench_Math
build/benchmarks/bin/UtilsBench_String
build/benchmarks/bin/UtilsBench_ProdStringsCase
# end::release_benchmarks[]

# tag::focused_benchmark[]
build/benchmarks/bin/UtilsBench_ProdStringsCase \
  --benchmark_filter='BM_(GoToLineStart|FindFirstWithLineLimit|GetLineNumberAndColumn)' \
  --benchmark_repetitions=10 \
  --benchmark_report_aggregates_only=true
# end::focused_benchmark[]

# tag::string_pool_lookup_benchmark[]
build/benchmarks/bin/UtilsBench_String \
  --benchmark_filter='BM_StringPool_(HashOnly|Multimap)_ExistingLookup' \
  --benchmark_repetitions=10 \
  --benchmark_enable_random_interleaving=true \
  --benchmark_report_aggregates_only=true
# end::string_pool_lookup_benchmark[]

# tag::format_check[]
clang-format --dry-run --Werror path/to/changed-file.cpp
# end::format_check[]

# tag::tidy_check[]
run-clang-tidy -p build -j 16
# end::tidy_check[]

# tag::valgrind_check[]
valgrind --leak-check=full --show-leak-kinds=definite \
  --track-origins=yes build/bin/UtilsTests
# end::valgrind_check[]
