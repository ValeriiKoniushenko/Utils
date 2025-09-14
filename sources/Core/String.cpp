//  MIT License
//
//  Copyright (c) 2019-2025 Valerii Koniushenko
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#include "String.h"

#include "Utils/EasyCSV.h"

namespace Core
{

#if defined(UTILS_DEBUG)
    StringTracer::~StringTracer()
    {
        saveMetrics();
    }

    void StringTracer::AddAtomRequest(std::string str)
    {
        _atomRequests[std::move(str)]++;
    }

    void StringTracer::AddAtomComparisonRequest(std::string str)
    {
        _atomCmpRequests[std::move(str)]++;
    }

    void StringTracer::AddDynamicComparisonRequest(std::string str)
    {
        _dynamicCmpRequests[std::move(str)]++;
    }

    void StringTracer::saveMetrics()
    {
        Utils::CSV csv;
        csv.setSavePath("string_metrics.csv");
        csv.addRow({ "String", "Atoms", "String", "AtomCmp", "String", "DynamicCmp" });

        for (auto&& [str, count] : _atomRequests)
        {
            csv.addToBottomOfColumn(0, { str, std::to_string(count) });
        }
        for (auto&& [str, count] : _atomCmpRequests)
        {
            csv.addToBottomOfColumn(2, { str, std::to_string(count) });
        }
        for (auto&& [str, count] : _dynamicCmpRequests)
        {
            csv.addToBottomOfColumn(4, { str, std::to_string(count) });
        }

        csv.save();
    }

#endif
} // namespace Core
