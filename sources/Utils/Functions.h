// MIT License
//
// Copyright (c) 2018-2025 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "libassert/assert.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

namespace Utils
{

    [[nodiscard]] bool IsReadable(const std::filesystem::path& p) noexcept;

    template<typename T, typename = void>
    struct __has_value_type : std::false_type
    {
    };

    template<typename T>
    struct __has_value_type<T, decltype(T::value_type, void())> : std::true_type
    {
    };

    // TODO: move in the future to the specialized class for working with FileSystem
    // clang-format off
    template<
        class T,
        bool ignoreAssert = false,
        class CharTypeT = std::conditional_t<__has_value_type<T>::value, typename T::value_type, char>
    >
    // clang-format on
    [[nodiscard]] T GetTextFileContentAs(const std::filesystem::path& path)
    {
        if (!IsReadable(path))
        {
            if constexpr (!ignoreAssert)
            {
                throw std::runtime_error("Impossible to open a file(permission error): " + path.generic_string());
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
        class CharTypeT = std::conditional_t<__has_value_type<T>::value, typename T::value_type, char>
    >
    // clang-format on
    [[nodiscard]] T TryToGetTextFileContentAs(const std::filesystem::path& path)
    {
        return GetTextFileContentAs<T, true, CharTypeT>(path);
    }

    [[nodiscard]] std::vector<char> GetFileContent(const std::filesystem::path& path);

} // namespace Utils
