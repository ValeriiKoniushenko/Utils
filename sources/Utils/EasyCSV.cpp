// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "EasyCSV.h"

#include <fstream>
#include <iostream>

namespace Utils
{

    void CSV::setSavePath(const std::filesystem::path& path)
    {
        _savePath = path;
    }

    void CSV::addRow(const std::vector<std::string>& row)
    {
        _table.emplace_back(row);
    }

    const std::vector<std::string>& CSV::getRow(std::size_t index) const
    {
        return _table.at(index);
    }

    void CSV::addToBottomOfColumn(std::size_t column, const std::string& value)
    {
        addToBottomOfColumn(column, std::vector{ value });
    }

    void CSV::addToBottomOfColumn(std::size_t column, const std::vector<std::string>& row)
    {
        if (row.empty())
        {
            return;
        }

        for (auto&& r : _table)
        {
            if (r.size() <= column + row.size())
            {
                r.resize(column + row.size());
            }

            if (r.at(column).empty())
            {
                for (std::size_t i = 0; i < row.size(); ++i)
                {
                    r.at(column + i) = row[i];
                }
                return;
            }
        }

        _table.emplace_back();
        if (_table.back().size() <= column)
        {
            _table.back().resize(column + row.size());
        }

        for (std::size_t i = 0; i < row.size(); ++i)
        {
            _table.back().at(column + i) = row[i];
        }
    }

    std::vector<std::string>& CSV::getRow(std::size_t index)
    {
        return _table.at(index);
    }

    void CSV::save(bool ignoreSpecChars) const
    {
        constexpr char delimiter = ';';

        std::ofstream file(_savePath);
        if (!file.is_open())
        {
            std::cerr << "Impossible to open a file: " << _savePath.generic_string() << '\n';
            return;
        }

        // cppcheck-suppress useStlAlgorithm
        for (const auto& row : _table)
        {
            // cppcheck-suppress useStlAlgorithm
            for (auto cell : row)
            {
                if (ignoreSpecChars)
                {
                    // cppcheck-suppress useStlAlgorithm
                    for (auto& c : cell)
                    {
                        if (c == delimiter || c < 32)
                        {
                            c = '_';
                        }
                    }
                }
                file << cell << delimiter;
            }
            file << '\n';
        }
    }

} // namespace Utils