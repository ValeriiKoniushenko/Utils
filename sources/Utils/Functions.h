// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "TypeTraits.h"

#include <filesystem>
#include <fstream>
#include <vector>

namespace Utils
{

    [[nodiscard]] bool IsReadable(const std::filesystem::path& p) noexcept;

    // clang-format off
    template<
        class T,
        bool ignoreAssert = false,
        class CharTypeT = std::conditional_t<HasValueType<T>::value, typename T::value_type, char>
    >
    // clang-format on
    [[nodiscard]] T GetTextFileContentAs(const std::filesystem::path& path)
    {
        if (!IsReadable(path))
        {
            if constexpr (!ignoreAssert)
            {
                throw std::runtime_error("Impossible to open a file(permission error): "
                                         + path.generic_string());
            }
            return {};
        }

        std::ifstream in(path);
        if (!in.is_open())
        {
            in.close();
            if constexpr (!ignoreAssert)
            {
                throw std::runtime_error("Impossible to open a file: " + path.generic_string());
            }
            return {};
        }

        return T{ std::istreambuf_iterator<CharTypeT>(in), std::istreambuf_iterator<CharTypeT>() };
    }

    // TODO: move in the future to the specialized class for working with FileSystem
    // clang-format off
    template<
        class T,
        class CharTypeT = std::conditional_t<HasValueType<T>::value, typename T::value_type, char>
    >
    // clang-format on
    [[nodiscard]] T TryToGetTextFileContentAs(const std::filesystem::path& path)
    {
        return GetTextFileContentAs<T, true, CharTypeT>(path);
    }

    [[nodiscard]] std::vector<char> GetFileContent(const std::filesystem::path& path);

} // namespace Utils
