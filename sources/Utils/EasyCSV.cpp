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

        for (const auto& row : _table)
        {
            for (auto cell : row)
            {
                if (ignoreSpecChars)
                {
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