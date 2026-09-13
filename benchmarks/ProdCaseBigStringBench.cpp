// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "Core/String.h"
#include "Dictionary.h"

#include <benchmark/benchmark.h>

using namespace Core;

namespace
{
    StringPool<char>& GetPool()
    {
        return StringPool<char>::Instance();
    }

    void BM_Peek(benchmark::State& state)
    {
        GetPool()._raw().clear();

        for (std::size_t i = 0; i < std::size(dictionary); ++i)
        {
            (void)GetPool().intern(dictionary[i], strlen(dictionary[i]));
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