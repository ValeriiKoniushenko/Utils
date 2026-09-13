// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

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
        [[nodiscard]] const std::filesystem::path& getSavePath() const noexcept
        {
            return _savePath;
        }

        void addRow(const std::vector<std::string>& row);
        [[nodiscard]] const std::vector<std::string>& getRow(std::size_t index) const;
        [[nodiscard]] const std::vector<std::string>& operator[](std::size_t index) const
        {
            return getRow(index);
        }

        void addToBottomOfColumn(std::size_t column, const std::string& value);
        void addToBottomOfColumn(std::size_t column, const std::vector<std::string>& row);

        [[nodiscard]] std::vector<std::string>& getRow(std::size_t index);
        [[nodiscard]] std::vector<std::string>& operator[](std::size_t index)
        {
            return getRow(index);
        }

        void save(bool ignoreSpecChars = true) const;

    protected:
        std::filesystem::path _savePath = "default.csv";
        std::vector<std::vector<std::string>> _table;
    };

} // namespace Utils