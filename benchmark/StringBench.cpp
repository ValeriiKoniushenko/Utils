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

#include "Dictionary.h"

#include <benchmark/benchmark.h>

using namespace Core;

namespace
{
    // -------------------------------
    // Static comparison
    // -------------------------------
    void BM_StdStringComparison(benchmark::State& state)
    {
        std::vector<std::string> set1;
        std::vector<std::string> set2;
        for (std::size_t i = 0; i < std::size(dictionary); ++i)
        {
            set1.push_back(dictionary[i]);
            set2.push_back(dictionary[i]);
        }

        for (auto _ : state)
        {
            for (std::size_t i = 0; i < std::size(dictionary); ++i)
            {
                benchmark::DoNotOptimize(set1[i] == set2[i]);
            }
        }
    }

    void BM_StringAtom_Static_Comparison(benchmark::State& state)
    {
        std::vector<StringAtom> set1;
        std::vector<StringAtom> set2;
        for (std::size_t i = 0; i < std::size(dictionary); ++i)
        {
            set1.push_back(StringAtom::Intern(dictionary[i]));
            set2.push_back(StringAtom::Intern(dictionary[i]));
        }

        for (auto _ : state)
        {
            for (std::size_t i = 0; i < std::size(dictionary); ++i)
            {
                benchmark::DoNotOptimize(set1[i] == set2[i]);
            }
        }
    }

    void BM_StringAtom_Dynamic_Comparison(benchmark::State& state)
    {
        std::vector<StringAtom> set1;
        std::vector<StringAtom> set2;
        for (std::size_t i = 0; i < std::size(dictionary); ++i)
        {
            set1.push_back(dictionary[i]);
            set2.push_back(dictionary[i]);
        }

        for (auto _ : state)
        {
            for (std::size_t i = 0; i < std::size(dictionary); ++i)
            {
                benchmark::DoNotOptimize(set1[i] == set2[i]);
            }
        }
    }

    // -------------------------------
    // PushingBack/Append
    // -------------------------------
    void BM_StringAtomPushingBack(benchmark::State& state)
    {
        for (auto _ : state)
        {
            StringAtom str;
            for (std::size_t i = 0; i < std::size(dictionary); ++i)
            {
                str.pushBack(dictionary[i]);
            }
            benchmark::ClobberMemory();
            benchmark::DoNotOptimize(str);
        }
    }

    void BM_StdStringPushingBack(benchmark::State& state)
    {
        for (auto _ : state)
        {
            std::string str;
            for (std::size_t i = 0; i < std::size(dictionary); ++i)
            {
                str.append(dictionary[i]);
            }
            benchmark::ClobberMemory();
            benchmark::DoNotOptimize(str);
        }
    }

    // -------------------------------
    // SUBSTRING
    // -------------------------------
    void BM_StringAtom_Substr(benchmark::State& state)
    {
        StringAtom str(std::string(state.range(0), 'a').c_str());
        for (auto _ : state)
        {
            str.subStr(0, state.range(0) / 2);
            benchmark::DoNotOptimize(str);
        }
        state.SetComplexityN(state.range(0));
    }

    void BM_StdString_Substr(benchmark::State& state)
    {
        std::string str(state.range(0), 'a');
        for (auto _ : state)
        {
            auto sub = str.substr(0, state.range(0) / 2);
            benchmark::DoNotOptimize(sub);
        }
        state.SetComplexityN(state.range(0));
    }

    // -------------------------------
    // FIND (linear search)
    // -------------------------------
    void BM_StringAtom_Find(benchmark::State& state)
    {
        StringAtom str(std::string(state.range(0), 'a').c_str());
        str.pushBack('b'); // ensure something to find
        for (auto _ : state)
        {
            auto pos = str.find("b");
            benchmark::DoNotOptimize(pos);
        }
        state.SetComplexityN(state.range(0));
    }

    void BM_StdString_Find(benchmark::State& state)
    {
        std::string str(state.range(0), 'a');
        str.push_back('b');
        for (auto _ : state)
        {
            auto pos = str.find('b');
            benchmark::DoNotOptimize(pos);
        }
        state.SetComplexityN(state.range(0));
    }

} // namespace

BENCHMARK(BM_StringAtomPushingBack);
BENCHMARK(BM_StdStringPushingBack);

BENCHMARK(BM_StdStringComparison);
BENCHMARK(BM_StringAtom_Static_Comparison);
BENCHMARK(BM_StringAtom_Dynamic_Comparison);

BENCHMARK(BM_StringAtom_Substr)->Range(8, 1 << 16)->Complexity();
BENCHMARK(BM_StdString_Substr)->Range(8, 1 << 16)->Complexity();

BENCHMARK(BM_StringAtom_Find)->Range(8, 1 << 16)->Complexity();
BENCHMARK(BM_StdString_Find)->Range(8, 1 << 16)->Complexity();

BENCHMARK_MAIN();