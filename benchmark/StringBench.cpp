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

#include "Core/String.h"

#ifdef _WIN32
#undef max
#undef min
#endif

#include <benchmark/benchmark.h>

static void BM_StdStringComparison(benchmark::State& state)
{
    std::string str1(state.range(0), 'a');
    std::string str2(state.range(0), 'a');

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(str1 == str2);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdStringPushingBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string str;
        for (std::size_t i = 0; i < state.range(0); ++i)
        {
            str.push_back('a');
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdStringPushingBack_LongString(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string str;

        for (std::size_t i = 0; i < state.range(0); ++i)
        {
            str.append("Hello");
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StringAtom_Dynamic_Comparison(benchmark::State& state)
{
    Core::StringAtom str1;
    for (int i = 0; i < state.range(0); ++i)
    {
        str1.push_back('a');
    }

    Core::StringAtom str2;
    for (int i = 0; i < state.range(0); ++i)
    {
        str2.push_back('a');
    }

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(str1 == str2);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdString_Dynamic_Comparison(benchmark::State& state)
{
    std::string str1;
    for (int i = 0; i < state.range(0); ++i)
    {
        str1.push_back('a');
    }

    std::string str2;
    for (int i = 0; i < state.range(0); ++i)
    {
        str2.push_back('a');
    }

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(str1 == str2);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StringAtom_Static_Comparison(benchmark::State& state)
{
    Core::StringAtom mainString;
    for (int i = 0; i < state.range(0); ++i)
    {
        mainString.push_back('a');
    }

    auto str1 = Core::StringAtom::Intern(mainString);
    auto str2 = Core::StringAtom::Intern(mainString);
    mainString.clear();

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(str1 == str2);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StringAtomPushingBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        Core::StringAtom str;
        for (std::size_t i = 0; i < state.range(0); ++i)
        {
            str.push_back('a');
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StringAtomPushingBack_LongString(benchmark::State& state)
{
    for (auto _ : state)
    {
        Core::StringAtom str;
        for (std::size_t i = 0; i < state.range(0); ++i)
        {
            str.push_back("Hello");
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_StringAtom_Dynamic_Comparison)->Range(2, 2 << 10)->Complexity();
BENCHMARK(BM_StdString_Dynamic_Comparison)->Range(2, 2 << 10)->Complexity();

// BENCHMARK(BM_StdStringComparison)->Range(2, 2 << 10)->Complexity();
// BENCHMARK(BM_StdStringPushingBack)->Range(2, 2 << 10)->Complexity();
// BENCHMARK(BM_StdStringPushingBack_LongString)->Range(2, 2 << 10)->Complexity();
//
// BENCHMARK(BM_StringAtom_Static_Comparison)->Range(2, 2 << 10)->Complexity();
// BENCHMARK(BM_StringAtomPushingBack)->Range(2, 2 << 10)->Complexity();
// BENCHMARK(BM_StringAtomPushingBack_LongString)->Range(2, 2 << 10)->Complexity();

BENCHMARK_MAIN();