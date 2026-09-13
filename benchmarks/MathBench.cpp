// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "Core/Math.h"

#ifdef _WIN32
    #undef max
    #undef min
#endif

#include <benchmark/benchmark.h>

namespace
{

    static void BM_StdAbs(benchmark::State& state)
    {
        for (auto _ : state)
        {
            benchmark::DoNotOptimize(std::abs(state.range(0)));
            benchmark::DoNotOptimize(std::abs(-state.range(0)));
            benchmark::DoNotOptimize(std::fabs(state.range(0)));
            benchmark::DoNotOptimize(std::fabs(-state.range(0)));
        }
        state.SetComplexityN(state.range(0));
    }

    static void BM_StdAbsFloat(benchmark::State& state)
    {
        for (auto _ : state)
        {
            benchmark::DoNotOptimize(std::abs((float)state.range(0)));
            benchmark::DoNotOptimize(std::abs((float)-state.range(0)));
            benchmark::DoNotOptimize(std::fabs((float)state.range(0)));
            benchmark::DoNotOptimize(std::fabs((float)-state.range(0)));
        }
        state.SetComplexityN(state.range(0));
    }

    static void BM_CoreAbs(benchmark::State& state)
    {
        for (auto _ : state)
        {
            benchmark::DoNotOptimize(::Math::Abs(state.range(0)));
            benchmark::DoNotOptimize(::Math::Abs(-state.range(0)));
            benchmark::DoNotOptimize(::Math::Abs(state.range(0)));
            benchmark::DoNotOptimize(::Math::Abs(-state.range(0)));
        }
        state.SetComplexityN(state.range(0));
    }

    static void BM_CoreAbsFloat(benchmark::State& state)
    {
        for (auto _ : state)
        {
            benchmark::DoNotOptimize(::Math::Abs((float)state.range(0)));
            benchmark::DoNotOptimize(::Math::Abs((float)-state.range(0)));
            benchmark::DoNotOptimize(::Math::Abs((float)state.range(0)));
            benchmark::DoNotOptimize(::Math::Abs((float)-state.range(0)));
        }
        state.SetComplexityN(state.range(0));
    }

} // namespace

BENCHMARK(BM_StdAbs)->Range(2, 2 << 16)->Complexity();
BENCHMARK(BM_CoreAbs)->Range(2, 2 << 16)->Complexity();
BENCHMARK(BM_StdAbsFloat)->Range(2, 2 << 16)->Complexity();
BENCHMARK(BM_CoreAbsFloat)->Range(2, 2 << 16)->Complexity();

BENCHMARK_MAIN();