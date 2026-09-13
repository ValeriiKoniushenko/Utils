// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "String.h"

#include <iostream>

#if defined(UTILS_DEBUG)

    #include "Utils/EasyCSV.h"

namespace Core
{

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

        auto add = [&csv](const char* first, const std::unordered_map<std::string, int64_t>& data)
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

} // namespace Core

#endif // defined(UTILS_DEBUG)

std::ostream& operator<<(std::ostream& stream, const Core::StringAtom& s)
{
    return stream << s.c_str();
}

std::istream& operator>>(std::istream& stream, Core::StringAtom& s)
{
    std::string temp;
    stream >> temp;
    s += temp;
    return stream;
}
