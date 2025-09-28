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

#include "Utils/CrossString.h"

#include <inttypes.h>
#include <iostream>
#include <optional>
#include <regex>
#include <unordered_map>

#define CreateEnum(Name, Type, ...)                                                                                                                  \
    struct Name final                                                                                                                                \
    {                                                                                                                                                \
    public:                                                                                                                                          \
        using SizeT = int32_t;                                                                                                                       \
        using KeyT = std::optional<Type>;                                                                                                            \
        using ValueT = std::string;                                                                                                                  \
        using UnderlyingType = Type;                                                                                                                 \
                                                                                                                                                     \
    public:                                                                                                                                          \
        Name(Type value)                                                                                                                             \
            : _value(value)                                                                                                                          \
        {                                                                                                                                            \
        }                                                                                                                                            \
                                                                                                                                                     \
        ~Name() = default;                                                                                                                           \
        Name(const Name&) = default;                                                                                                                 \
        Name(Name&&) = default;                                                                                                                      \
        Name& operator=(Name&&) = default;                                                                                                           \
                                                                                                                                                     \
        [[nodiscard]] bool operator==(Name other) const noexcept { return _value == other._value; }                                                  \
                                                                                                                                                     \
        Name& operator=(Name other) noexcept                                                                                                         \
        {                                                                                                                                            \
            _value = other._value;                                                                                                                   \
            return *this;                                                                                                                            \
        }                                                                                                                                            \
                                                                                                                                                     \
        Name& operator=(Type value) noexcept                                                                                                         \
        {                                                                                                                                            \
            _value = value;                                                                                                                          \
            return *this;                                                                                                                            \
        }                                                                                                                                            \
                                                                                                                                                     \
        [[nodiscard]] bool operator!=(Name other) const noexcept { return _value != other._value; }                                                  \
                                                                                                                                                     \
        [[nodiscard]] Type cast() const noexcept { return _value; }                                                                                  \
                                                                                                                                                     \
        [[nodiscard]] operator Type() const noexcept { return _value; }                                                                              \
                                                                                                                                                     \
        [[nodiscard]] ValueT toStr() const { return Name::toStr(_value); }                                                                           \
                                                                                                                                                     \
        enum : Type                                                                                                                                  \
        {                                                                                                                                            \
            __VA_ARGS__                                                                                                                              \
        };                                                                                                                                           \
        inline static const SizeT count = []() -> Name::SizeT                                                                                        \
        {                                                                                                                                            \
            std::regex const expression(",");                                                                                                        \
            ValueT const text(#__VA_ARGS__);                                                                                                         \
            std::ptrdiff_t const match_count(std::distance(std::sregex_iterator(text.begin(), text.end(), expression), std::sregex_iterator()));     \
            return static_cast<Name::SizeT>(match_count + 1);                                                                                        \
        }();                                                                                                                                         \
                                                                                                                                                     \
        [[nodiscard]] static ValueT toStr(Type key)                                                                                                  \
        {                                                                                                                                            \
            auto it = map.find(KeyT(key));                                                                                                           \
            if (it == map.end())                                                                                                                     \
            {                                                                                                                                        \
                static const bool _ = [key]()                                                                                                        \
                {                                                                                                                                    \
                    std::cerr << "Impossible to find a way to convert a value '" << key << "' to string inside the enum: '" << Name::getName()       \
                              << "'" << std::endl;                                                                                                   \
                    return true;                                                                                                                     \
                }();                                                                                                                                 \
                return {};                                                                                                                           \
            }                                                                                                                                        \
                                                                                                                                                     \
            return it->second;                                                                                                                       \
        }                                                                                                                                            \
                                                                                                                                                     \
        static KeyT fromStr(const ValueT& value)                                                                                                     \
        {                                                                                                                                            \
            auto it = std::ranges::find_if(map,                                                                                                      \
                                           [&value](auto pair)                                                                                       \
                                           {                                                                                                         \
                                               return pair.second == value;                                                                          \
                                           });                                                                                                       \
                                                                                                                                                     \
            if (it == map.end())                                                                                                                     \
            {                                                                                                                                        \
                static const bool _ = [&value]()                                                                                                     \
                {                                                                                                                                    \
                    std::cerr << "Impossible to find a way to convert a string '" << value << "' to string inside the enum: '" << Name::getName()    \
                              << "'" << std::endl;                                                                                                   \
                    return true;                                                                                                                     \
                }();                                                                                                                                 \
                return {};                                                                                                                           \
            }                                                                                                                                        \
                                                                                                                                                     \
            return it->first;                                                                                                                        \
        }                                                                                                                                            \
                                                                                                                                                     \
        [[nodiscard]] static constexpr std::string getName() noexcept { return #Name; }                                                              \
                                                                                                                                                     \
    private:                                                                                                                                         \
        Type _value{};                                                                                                                               \
        using ContainerT = std::unordered_map<KeyT, ValueT>;                                                                                         \
        inline static const ValueT text = #__VA_ARGS__;                                                                                              \
        inline static const ContainerT map = []()                                                                                                    \
        {                                                                                                                                            \
            std::regex tokenRegex("[A-Z]([\\w\\s =])*,");                                                                                            \
            std::string temp = text + ",";                                                                                                           \
            std::vector<std::string> tokens;                                                                                                         \
                                                                                                                                                     \
            for (std::sregex_iterator i = std::sregex_iterator(temp.begin(), temp.end(), tokenRegex); i != std::sregex_iterator(); ++i)              \
            {                                                                                                                                        \
                tokens.emplace_back(std::smatch(*i).str());                                                                                          \
            }                                                                                                                                        \
                                                                                                                                                     \
            ContainerT data;                                                                                                                         \
            Type counter{};                                                                                                                          \
            bool isCouldntParseInThePast = false;                                                                                                    \
                                                                                                                                                     \
            for (auto token : tokens)                                                                                                                \
            {                                                                                                                                        \
                std::regex subTokenRegex("^[A-Z]\\w*");                                                                                              \
                auto iter = std::sregex_iterator(token.begin(), token.end(), subTokenRegex);                                                         \
                if (iter == std::sregex_iterator())                                                                                                  \
                {                                                                                                                                    \
                    continue;                                                                                                                        \
                }                                                                                                                                    \
                                                                                                                                                     \
                const ValueT value = std::smatch(*iter).str();                                                                                       \
                KeyT key;                                                                                                                            \
                if (!isCouldntParseInThePast)                                                                                                        \
                {                                                                                                                                    \
                    key = counter;                                                                                                                   \
                }                                                                                                                                    \
                                                                                                                                                     \
                if (auto pos = token.find_first_of('='); pos != std::string::npos)                                                                   \
                {                                                                                                                                    \
                    ++pos;                                                                                                                           \
                    for (; std::isspace(token[pos]) && pos < token.size(); ++pos)                                                                    \
                    {                                                                                                                                \
                    }                                                                                                                                \
                    token = token.substr(pos);                                                                                                       \
                                                                                                                                                     \
                    std::regex valueRegex("^[0-9]+,?$");                                                                                             \
                    if (!std::regex_match(token.begin(), token.end(), valueRegex))                                                                   \
                    {                                                                                                                                \
                        key.reset();                                                                                                                 \
                        isCouldntParseInThePast = true;                                                                                              \
                    }                                                                                                                                \
                    else                                                                                                                             \
                    {                                                                                                                                \
                        key = counter = static_cast<Type>(atoll(token.c_str()));                                                                     \
                        isCouldntParseInThePast = false;                                                                                             \
                    }                                                                                                                                \
                }                                                                                                                                    \
                ++counter;                                                                                                                           \
                                                                                                                                                     \
                data.emplace(key, value);                                                                                                            \
            }                                                                                                                                        \
                                                                                                                                                     \
            return data;                                                                                                                             \
        }();                                                                                                                                         \
    };
