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

#include <algorithm>
#include <benchmark/benchmark.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

using namespace Core;

namespace
{
    using PoolHashT = std::size_t;
    using PoolValueT = StringData<char>;

    [[nodiscard]] PoolHashT MakePoolHash(std::string_view value)
    {
        return StringAtom::Toolset::Hash(value.data(), value.size());
    }

    [[nodiscard]] PoolValueT MakePoolValue(std::string_view value)
    {
        std::vector<char> buffer(value.size() + 1);
        std::ranges::copy(value, buffer.begin());
        buffer[value.size()] = 0;
        return { std::move(buffer), value.size() };
    }

    [[nodiscard]] std::vector<std::string> MakeUniqueHashCorpus(std::size_t count)
    {
        std::vector<std::string> corpus;
        std::unordered_set<PoolHashT> hashes;
        corpus.reserve(count);
        hashes.reserve(count);

        for (std::size_t index = 0; corpus.size() < count; ++index)
        {
            auto value = "Utils.StringPool.Lookup." + std::to_string(index);
            if (hashes.emplace(MakePoolHash(value)).second)
            {
                corpus.push_back(std::move(value));
            }
        }

        return corpus;
    }

    class HashOnlyLookupPool
    {
    public:
        explicit HashOnlyLookupPool(const std::vector<std::string>& values)
        {
            _strings.reserve(values.size());
            for (const auto& value : values)
            {
                _strings.emplace(MakePoolHash(value), MakePoolValue(value));
            }
        }

        [[nodiscard]] const char* findExisting(std::string_view value) const
        {
            const auto it = _strings.find(MakePoolHash(value));
            return it == _strings.end() ? nullptr : it->second.str.data();
        }

    private:
        // The original StringPool lookup: a hash is treated as the string identity.
        std::unordered_map<PoolHashT, PoolValueT> _strings;
    };

    class CollisionSafeLookupPool
    {
    public:
        explicit CollisionSafeLookupPool(const std::vector<std::string>& values)
        {
            _strings.reserve(values.size());
            for (const auto& value : values)
            {
                _strings.emplace(MakePoolHash(value), MakePoolValue(value));
            }
        }

        [[nodiscard]] const char* findExisting(std::string_view value) const
        {
            const auto [first, last] = _strings.equal_range(MakePoolHash(value));
            for (auto it = first; it != last; ++it)
            {
                const auto& existing = it->second;
                if (existing.size == value.size()
                    && std::equal(existing.str.begin(), existing.str.end() - 1, value.begin()))
                {
                    return existing.str.data();
                }
            }

            return nullptr;
        }

    private:
        // The current StringPool lookup: hash narrows candidates, bytes identify the string.
        std::unordered_multimap<PoolHashT, PoolValueT> _strings;
    };

    template<class PoolT>
    void BenchmarkExistingPoolLookup(benchmark::State& state)
    {
        const auto corpus = MakeUniqueHashCorpus(static_cast<std::size_t>(state.range(0)));
        const PoolT pool(corpus);
        auto value = corpus.begin();

        for (auto _ : state)
        {
            benchmark::DoNotOptimize(pool.findExisting(*value));
            if (++value == corpus.end())
            {
                value = corpus.begin();
            }
        }
    }

    void BM_StringPool_HashOnly_ExistingLookup(benchmark::State& state)
    {
        BenchmarkExistingPoolLookup<HashOnlyLookupPool>(state);
    }

    void BM_StringPool_Multimap_ExistingLookup(benchmark::State& state)
    {
        BenchmarkExistingPoolLookup<CollisionSafeLookupPool>(state);
    }

    // -------------------------------
    // Static comparison
    // -------------------------------
    void BM_StdStringComparison(benchmark::State& state)
    {
        std::vector<std::string> set1;
        std::vector<std::string> set2;
        for (const auto i : dictionary)
        {
            set1.push_back(i);
            set2.push_back(i);
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
        for (const auto i : dictionary)
        {
            set1.push_back(StringAtom::Intern(i));
            set2.push_back(StringAtom::Intern(i));
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
        for (const auto i : dictionary)
        {
            set1.emplace_back(i);
            set2.emplace_back(i);
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
            for (const auto i : dictionary)
            {
                str.pushBack(i);
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
            for (const auto i : dictionary)
            {
                str.append(i);
            }
            benchmark::ClobberMemory();
            benchmark::DoNotOptimize(str);
        }
    }

    // -------------------------------
    // PushingFront/Prepend
    // -------------------------------
    void BM_StringAtomPushingFront(benchmark::State& state)
    {
        for (auto _ : state)
        {
            StringAtom str;
            for (const auto i : dictionary)
            {
                str.pushFront(i);
            }
            benchmark::ClobberMemory();
            benchmark::DoNotOptimize(str);
        }
    }

    void BM_StdStringPushingFront(benchmark::State& state)
    {
        for (auto _ : state)
        {
            std::string str;
            for (const auto i : dictionary)
            {
                str.insert(0, i);
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

BENCHMARK(BM_StringAtomPushingFront);
BENCHMARK(BM_StdStringPushingFront);

BENCHMARK(BM_StringAtomPushingBack);
BENCHMARK(BM_StdStringPushingBack);

BENCHMARK(BM_StdStringComparison);
BENCHMARK(BM_StringAtom_Static_Comparison);
BENCHMARK(BM_StringAtom_Dynamic_Comparison);

BENCHMARK(BM_StringAtom_Substr)->Range(8, 1 << 16)->Complexity();
BENCHMARK(BM_StdString_Substr)->Range(8, 1 << 16)->Complexity();

BENCHMARK(BM_StringAtom_Find)->Range(8, 1 << 16)->Complexity();
BENCHMARK(BM_StdString_Find)->Range(8, 1 << 16)->Complexity();

BENCHMARK(BM_StringPool_HashOnly_ExistingLookup)->Arg(1'000)->Arg(10'000);
BENCHMARK(BM_StringPool_Multimap_ExistingLookup)->Arg(1'000)->Arg(10'000);

BENCHMARK_MAIN();
