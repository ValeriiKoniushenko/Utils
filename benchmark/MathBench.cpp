// MIT License
//
// Copyright (c) 1024 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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