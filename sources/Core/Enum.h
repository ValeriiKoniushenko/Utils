// MIT License
//
// Copyright (c) 2024 Valerii Koniushenko
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

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

#define CreateEnum(Name, Type, ...)                                                                                                                  \
    struct Name                                                                                                                                      \
    {                                                                                                                                                \
    private:                                                                                                                                         \
        using SizeT = __int32;                                                                                                                       \
        using KeyT = std::optional<Type>;                                                                                                            \
        using ValueT = std::string;                                                                                                                  \
                                                                                                                                                     \
    public:                                                                                                                                          \
        Name(Type value)                                                                                                                             \
            : mValue(value)                                                                                                                          \
        {                                                                                                                                            \
        }                                                                                                                                            \
        ~Name() = default;                                                                                                                           \
        Name(const Name&) = default;                                                                                                                 \
        Name(Name&&) = default;                                                                                                                      \
        Name& operator=(Name&&) = default;                                                                                                           \
                                                                                                                                                     \
        bool operator==(Name other) const                                                                                                            \
        {                                                                                                                                            \
            return mValue == other.mValue;                                                                                                           \
        }                                                                                                                                            \
        Name& operator=(Name other)                                                                                                                  \
        {                                                                                                                                            \
            mValue = other.mValue;                                                                                                                   \
            return *this;                                                                                                                            \
        }                                                                                                                                            \
        Name& operator=(Type value)                                                                                                                  \
        {                                                                                                                                            \
            mValue = value;                                                                                                                          \
            return *this;                                                                                                                            \
        }                                                                                                                                            \
        bool operator!=(Name other) const                                                                                                            \
        {                                                                                                                                            \
            return mValue != other.mValue;                                                                                                           \
        }                                                                                                                                            \
        Type Cast() const                                                                                                                            \
        {                                                                                                                                            \
            return mValue;                                                                                                                           \
        }                                                                                                                                            \
        ValueT ToStr() const                                                                                                                         \
        {                                                                                                                                            \
            return Name::ToStr(mValue);                                                                                                              \
        }                                                                                                                                            \
                                                                                                                                                     \
        enum : Type                                                                                                                                  \
        {                                                                                                                                            \
            __VA_ARGS__                                                                                                                              \
        };                                                                                                                                           \
        inline static const SizeT Count = []() -> Name::SizeT                                                                                        \
        {                                                                                                                                            \
            std::regex const expression(",");                                                                                                        \
            ValueT const text(#__VA_ARGS__);                                                                                                         \
            std::ptrdiff_t const match_count(std::distance(std::sregex_iterator(text.begin(), text.end(), expression), std::sregex_iterator()));     \
            return static_cast<Name::SizeT>(match_count + 1);                                                                                        \
        }();                                                                                                                                         \
                                                                                                                                                     \
        static ValueT ToStr(Type key)                                                                                                                \
        {                                                                                                                                            \
            auto it = map.find(KeyT(key));                                                                                                           \
            if (it == map.end())                                                                                                                     \
            {                                                                                                                                        \
                std::cerr << "Impossible to find a way to convert a value '" << key << "' to string inside the enum: '" << Name::GetName() << "'"    \
                          << std::endl;                                                                                                              \
                return {};                                                                                                                           \
            }                                                                                                                                        \
                                                                                                                                                     \
            return it->second;                                                                                                                       \
        }                                                                                                                                            \
                                                                                                                                                     \
        static KeyT FromStr(const ValueT& value)                                                                                                     \
        {                                                                                                                                            \
            auto it = std::find_if(map.begin(), map.end(),                                                                                           \
                                   [&value](auto pair)                                                                                               \
                                   {                                                                                                                 \
                                       return pair.second == value;                                                                                  \
                                   });                                                                                                               \
                                                                                                                                                     \
            if (it == map.end())                                                                                                                     \
            {                                                                                                                                        \
                std::cerr << "Impossible to find a way to convert a string '" << value << "' to string inside the enum: '" << Name::GetName() << "'" \
                          << std::endl;                                                                                                              \
                return {};                                                                                                                           \
            }                                                                                                                                        \
                                                                                                                                                     \
            return it->first;                                                                                                                        \
        }                                                                                                                                            \
                                                                                                                                                     \
        [[nodiscard]] static constexpr std::string GetName() noexcept                                                                                \
        {                                                                                                                                            \
            return #Name;                                                                                                                            \
        }                                                                                                                                            \
                                                                                                                                                     \
    private:                                                                                                                                         \
        Type mValue{};                                                                                                                               \
        using ContainerT = std::unordered_map<KeyT, ValueT>;                                                                                         \
        inline static const ValueT text = #__VA_ARGS__;                                                                                              \
        inline static const ContainerT map = []()                                                                                                    \
        {                                                                                                                                            \
            std::regex r("[A-Z]([\\w\\s =])*,");                                                                                                     \
            std::string temp = text + ",";                                                                                                           \
                                                                                                                                                     \
            std::vector<std::string> tokens;                                                                                                         \
                                                                                                                                                     \
            for (std::sregex_iterator i = std::sregex_iterator(temp.begin(), temp.end(), r); i != std::sregex_iterator(); ++i)                       \
            {                                                                                                                                        \
                tokens.emplace_back(std::smatch(*i).str());                                                                                          \
            }                                                                                                                                        \
                                                                                                                                                     \
            ContainerT data;                                                                                                                         \
            Type counter{};                                                                                                                          \
            bool isCouldntParseInThePast = false;                                                                                                    \
                                                                                                                                                     \
            for (auto& token : tokens)                                                                                                               \
            {                                                                                                                                        \
                std::regex r("[A-Z]\\w*");                                                                                                           \
                auto it = std::sregex_iterator(token.begin(), token.end(), r);                                                                       \
                if (it == std::sregex_iterator())                                                                                                    \
                {                                                                                                                                    \
                    std::cerr << "Can't find a property inside an enum: '" << Name::GetName() << "'" << std::endl;                                   \
                    continue;                                                                                                                        \
                }                                                                                                                                    \
                                                                                                                                                     \
                const ValueT value = std::smatch(*it).str();                                                                                         \
                KeyT key;                                                                                                                            \
                if (!isCouldntParseInThePast)                                                                                                        \
                {                                                                                                                                    \
                    key = counter;                                                                                                                   \
                }                                                                                                                                    \
                                                                                                                                                     \
                if (token.find_first_of('=') != std::string::npos)                                                                                   \
                {                                                                                                                                    \
                    std::regex r("[0-9]+");                                                                                                          \
                    auto it = std::sregex_iterator(token.begin(), token.end(), r);                                                                   \
                    if (it == std::sregex_iterator())                                                                                                \
                    {                                                                                                                                \
                        key.reset();                                                                                                                 \
                        std::cerr << "Can't parse a property's('" << value << "') value inside the enum: '" << Name::GetName() << "'" << std::endl;  \
                        isCouldntParseInThePast = true;                                                                                              \
                    }                                                                                                                                \
                    else                                                                                                                             \
                    {                                                                                                                                \
                        key = counter = static_cast<Type>(atoll(std::smatch(*it).str().c_str()));                                                    \
                        isCouldntParseInThePast = false;                                                                                             \
                    }                                                                                                                                \
                }                                                                                                                                    \
                else if (isCouldntParseInThePast)                                                                                                    \
                {                                                                                                                                    \
                    std::cerr << "Can't parse a property's('" << value << "') value inside the enum: '" << Name::GetName() << "'" << std::endl;      \
                }                                                                                                                                    \
                                                                                                                                                     \
                ++counter;                                                                                                                           \
                                                                                                                                                     \
                data.emplace(key, value);                                                                                                            \
            }                                                                                                                                        \
                                                                                                                                                     \
            return data;                                                                                                                             \
        }();                                                                                                                                         \
    };