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
#include "Dictionary.h"

#include <benchmark/benchmark.h>

using namespace Core;

namespace
{
    StringPool<char>& GetPool()
    {
        return StringPool<char>::instance();
    }

    void BM_Peek(benchmark::State& state)
    {
        GetPool()._raw().clear();

        for (std::size_t i = 0; i < std::size(dictionary); ++i)
        {
            (void)GetPool().add(dictionary[i], strlen(dictionary[i]));
        }

        for (auto _ : state)
        {
            auto a = "welcomed"_atom;
            benchmark::DoNotOptimize(a);

            auto b = "pleasures"_atom;
            benchmark::DoNotOptimize(b);

            auto c = "circumstances"_atom;
            benchmark::DoNotOptimize(c);

            auto d = "obligations"_atom;
            benchmark::DoNotOptimize(d);

            auto e = "!!! denouncing who are you"_atom;
            benchmark::DoNotOptimize(e);
            benchmark::DoNotOptimize(e.size());
        }
    }
    void BM_StdPeek(benchmark::State& state)
    {
        for (auto _ : state)
        {
            std::string a = "welcomed";
            benchmark::DoNotOptimize(a);

            std::string b = "pleasures";
            benchmark::DoNotOptimize(b);

            std::string c = "circumstances";
            benchmark::DoNotOptimize(c);

            std::string d = "obligations";
            benchmark::DoNotOptimize(d);

            std::string e = "!!! denouncing who are you";
            benchmark::DoNotOptimize(e);
            benchmark::DoNotOptimize(e.size());
        }
    }

    void BM_StdStringHash(benchmark::State& state)
    {
        for (auto _ : state)
        {
            for (std::size_t i = 0; i < std::size(dictionary); ++i)
            {
                benchmark::DoNotOptimize(std::hash<std::string>()(dictionary[i]));
            }
        }
    }
    void BM_StdStringViewHash(benchmark::State& state)
    {
        for (auto _ : state)
        {
            for (std::size_t i = 0; i < std::size(dictionary); ++i)
            {
                benchmark::DoNotOptimize(std::hash<std::string_view>()(dictionary[i]));
            }
        }
    }
    void BM_Fvn1aHash(benchmark::State& state)
    {
        for (auto _ : state)
        {
            for (std::size_t i = 0; i < std::size(dictionary); ++i)
            {
                benchmark::DoNotOptimize(StringAtom::Toolset::Hash(dictionary[i]));
            }
        }
    }

} // namespace

BENCHMARK(BM_StdStringHash);
BENCHMARK(BM_StdStringViewHash);
BENCHMARK(BM_Fvn1aHash);

BENCHMARK(BM_Peek);
BENCHMARK(BM_StdPeek);

BENCHMARK_MAIN();