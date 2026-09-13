// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "Functions.h"

namespace Utils
{

    bool IsReadable(const std::filesystem::path& p) noexcept
    {
        using namespace std::filesystem;

        std::error_code ec; // For noexcept overload usage.
        const auto perms = status(p, ec).permissions();
        return (perms & perms::owner_read) != perms::none
               && (perms & perms::group_read) != perms::none
               && (perms & perms::others_read) != perms::none;
    }

    std::vector<char> GetFileContent(const std::filesystem::path& path)
    {
        std::vector<char> out;

        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Impossible to open a file: " + path.generic_string());
        }

        const auto size = file.tellg();
        out.resize(size);
        file.seekg(0, std::ios_base::beg);
        file.read(out.data(), size);
        file.close();

        return out;
    }

} // namespace Utils
