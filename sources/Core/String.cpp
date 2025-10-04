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

#include "String.h"

#include "Utils/EasyCSV.h"

#include <iostream>

namespace Core
{

#if defined(UTILS_DEBUG)
    StringTracer::~StringTracer()
    {
        saveMetrics();
    }

    void StringTracer::addAtomRequest(std::string str)
    {
        _atomRequests[std::move(str)]++;
    }

    void StringTracer::addAtomComparisonRequest(std::string str)
    {
        _atomCmpRequests[std::move(str)]++;
    }

    void StringTracer::addDynamicComparisonRequest(std::string str)
    {
        _dynamicCmpRequests[std::move(str)]++;
    }

    void StringTracer::addChangedPolicyToDynamic(std::string str)
    {
        _changedPolicyToDynamic[std::move(str)]++;
    }

    void StringTracer::saveMetrics()
    {
        Utils::CSV csv;
        csv.setSavePath("string_metrics.csv");

        auto add = [&csv](std::string first, const std::unordered_map<std::string, int64_t>& data)
        {
            static int i = 0;

            csv.addToBottomOfColumn(i, first);
            for (auto&& [str, count] : data)
            {
                csv.addToBottomOfColumn(i, { str, std::to_string(count) });
            }

            i += 3;
        };

        add("Pure static", _atomRequests);
        add("Static compared", _atomCmpRequests);
        add("Dynamic compared", _dynamicCmpRequests);
        add("Converted to dynamic", _changedPolicyToDynamic);

        csv.save();
    }

#endif

} // namespace Core

std::ostream& operator<<(std::ostream& stream, const Core::StringAtom& s)
{
    return stream << s.c_str();
}

std::istream& operator>>(std::istream& stream, Core::StringAtom& s)
{
    std::string temp;
    std::cin >> temp;
    s += temp;
    return stream;
}
