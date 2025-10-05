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

#pragma once
#include "CopyableAndMoveableBehaviour.h"

#include <filesystem>
#include <vector>

namespace Utils
{

    class CSV
    {
    public:
        CSV() = default;
        ~CSV() = default;
        CSV(const CSV&) = default;
        CSV(CSV&&) = default;
        CSV& operator=(const CSV&) = default;
        CSV& operator=(CSV&&) = default;

        void setSavePath(const std::filesystem::path& path);
        [[nodiscard]] const std::filesystem::path& getSavePath() const noexcept { return _savePath; }

        void addRow(const std::vector<std::string>& row);
        [[nodiscard]] const std::vector<std::string>& getRow(std::size_t index) const;
        [[nodiscard]] const std::vector<std::string>& operator[](std::size_t index) const { return getRow(index); }

        void addToBottomOfColumn(std::size_t column, const std::string& value);
        void addToBottomOfColumn(std::size_t column, const std::vector<std::string>& row);

        [[nodiscard]] std::vector<std::string>& getRow(std::size_t index);
        [[nodiscard]] std::vector<std::string>& operator[](std::size_t index) { return getRow(index); }

        void save(bool ignoreSpecChars = true) const;

    protected:
        std::filesystem::path _savePath = "default.csv";
        std::vector<std::vector<std::string>> _table;
    };

} // namespace Utils