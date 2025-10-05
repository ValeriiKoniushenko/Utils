/*
 * MIT License
 *
 * Copyright (c) 2018-2025 Valerii Koniushenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Core/String.h"

#ifdef _WIN32
    #undef max
    #undef min
#endif

#include <benchmark/benchmark.h>

// -------------------------------
// Static comparison
// -------------------------------
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

static void BM_StringAtom_Static_Comparison(benchmark::State& state)
{
    Core::StringAtom mainString;
    for (int64_t i = 0; i < state.range(0); ++i)
    {
        mainString.pushBack('a');
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

// -------------------------------
// Dynamic comparison
// -------------------------------
static void BM_StdString_Dynamic_Comparison(benchmark::State& state)
{
    std::string str1;
    for (int64_t i = 0; i < state.range(0); ++i)
    {
        str1.push_back('a');
    }

    std::string str2;
    for (int64_t i = 0; i < state.range(0); ++i)
    {
        str2.push_back('a');
    }

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(str1 == str2);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StringAtom_Dynamic_Comparison(benchmark::State& state)
{
    Core::StringAtom str1;
    for (int64_t i = 0; i < state.range(0); ++i)
    {
        str1.pushBack('a');
    }

    Core::StringAtom str2;
    for (int64_t i = 0; i < state.range(0); ++i)
    {
        str2.pushBack('a');
    }

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(str1 == str2);
    }
    state.SetComplexityN(state.range(0));
}

// -------------------------------
// PushingBack
// -------------------------------
static void BM_StringAtomPushingBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        Core::StringAtom str;
        for (int64_t i = 0; i < state.range(0); ++i)
        {
            str.pushBack('a');
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdStringPushingBack(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string str;
        for (int64_t i = 0; i < state.range(0); ++i)
        {
            str.push_back('a');
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

// -------------------------------
// PushingBack LongString
// -------------------------------
static void BM_StdStringPushingBack_LongString(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string str;

        for (int64_t i = 0; i < state.range(0); ++i)
        {
            str.append("Hello");
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
        for (int64_t i = 0; i < state.range(0); ++i)
        {
            str.pushBack("Hello");
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

// -------------------------------
// APPEND (concatenate another string)
// -------------------------------
static void BM_StringAtom_Append(benchmark::State& state) {
    Core::StringAtom base("hello");
    Core::StringAtom toAdd(" world");
    for (auto _ : state) {
        Core::StringAtom str = base;
        for (int64_t i = 0; i < state.range(0); ++i) {
            str += toAdd;
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdString_Append(benchmark::State& state) {
    std::string base("hello");
    std::string toAdd(" world");
    for (auto _ : state) {
        std::string str = base;
        for (int64_t i = 0; i < state.range(0); ++i) {
            str += toAdd;
        }
        benchmark::DoNotOptimize(str);
    }
    state.SetComplexityN(state.range(0));
}

// -------------------------------
// SUBSTRING
// -------------------------------
static void BM_StringAtom_Substr(benchmark::State& state) {
    Core::StringAtom str(std::string(state.range(0), 'a').c_str());
    for (auto _ : state) {
        auto sub = str.subStr(0, state.range(0) / 2);
        benchmark::DoNotOptimize(sub);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdString_Substr(benchmark::State& state) {
    std::string str(state.range(0), 'a');
    for (auto _ : state) {
        auto sub = str.substr(0, state.range(0) / 2);
        benchmark::DoNotOptimize(sub);
    }
    state.SetComplexityN(state.range(0));
}

// -------------------------------
// FIND (linear search)
// -------------------------------
static void BM_StringAtom_Find(benchmark::State& state) {
    Core::StringAtom str(std::string(state.range(0), 'a').c_str());
    str.pushBack('b'); // ensure something to find
    for (auto _ : state) {
        auto pos = str.find("b");
        benchmark::DoNotOptimize(pos);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdString_Find(benchmark::State& state) {
    std::string str(state.range(0), 'a');
    str.push_back('b');
    for (auto _ : state) {
        auto pos = str.find('b');
        benchmark::DoNotOptimize(pos);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_StdStringComparison)->Range(2, 2 << 10)->Complexity();
BENCHMARK(BM_StringAtom_Static_Comparison)->Range(2, 2 << 10)->Complexity();

BENCHMARK(BM_StringAtom_Dynamic_Comparison)->Range(2, 2 << 10)->Complexity();
BENCHMARK(BM_StdString_Dynamic_Comparison)->Range(2, 2 << 10)->Complexity();

BENCHMARK(BM_StringAtomPushingBack)->Range(2, 2 << 10)->Complexity();
BENCHMARK(BM_StdStringPushingBack)->Range(2, 2 << 10)->Complexity();

BENCHMARK(BM_StringAtomPushingBack_LongString)->Range(2, 2 << 12)->Complexity();
BENCHMARK(BM_StdStringPushingBack_LongString)->Range(2, 2 << 12)->Complexity();

BENCHMARK(BM_StringAtom_Append)->Range(8, 1 << 10)->Complexity();
BENCHMARK(BM_StdString_Append)->Range(8, 1 << 10)->Complexity();

BENCHMARK(BM_StringAtom_Substr)->Range(8, 1 << 16)->Complexity();
BENCHMARK(BM_StdString_Substr)->Range(8, 1 << 16)->Complexity();

BENCHMARK(BM_StringAtom_Find)->Range(8, 1 << 16)->Complexity();
BENCHMARK(BM_StdString_Find)->Range(8, 1 << 16)->Complexity();


BENCHMARK_MAIN();