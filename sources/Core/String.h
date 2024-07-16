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

#include "Assert.h"
#include "GlobalEnums.h"
#include "Utils/CopyableAndMoveableBehaviour.h"

#include <optional>
#include <regex>
#include <string_view>
#include <type_traits>
#include <vector>
#include <xstring>

namespace Core
{
    enum class StringPolicy
    {
        Static,
        Dynamic
    };

    template<class CharType>
    struct _StringToolset : public Utils::Abstract
    {
        static_assert(false, "Template specialization for such type wasn't implemented.");
    };

    template<>
    struct _StringToolset<char> : public Utils::Abstract
    {
        using CharT = char;
        using StringT = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StringViewT = std::basic_string_view<CharT>;

        static int ToInt(const CharT* str) { return atoi(str); }
        static float ToFloat(const CharT* str) { return static_cast<float>(atof(str)); }
        static double ToDouble(const CharT* str) { return atof(str); }
        static long long ToLongLong(const CharT* str) { return atof(str); }
        static Comparison Cmp(const CharT* str1, const CharT* str2)
        {
            const int result = strcmp(str1, str2);
            if (result == 0)
            {
                return Comparison::Equal;
            }
            if (result > 0)
            {
                return Comparison::Greater;
            }

            return Comparison::Less;
        }
    };

    template<>
    struct _StringToolset<wchar_t> : public Utils::Abstract
    {
        using CharT = wchar_t;
        using StringT = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StringViewT = std::basic_string_view<CharT>;
        static int ToInt(const CharT* str) { return _wtoi(str); }
        static float ToFloat(const CharT* str) { return static_cast<float>(_wtof(str)); }
        static double ToDouble(const CharT* str) { return _wtof(str); }
        static long long ToLongLong(const CharT* str) { return _wtoll(str); }
        static Comparison Cmp(const CharT* str1, const CharT* str2)
        {
            const int result = wcscmp(str1, str2);
            if (result == 0)
            {
                return Comparison::Equal;
            }
            if (result > 0)
            {
                return Comparison::Greater;
            }

            return Comparison::Less;
        }
    };

    template<class CharType>
    class IString : public Utils::CopyableAndMoveable
    {
    public:
        using CharT = CharType;
        using Toolset = _StringToolset<CharT>;
        using SizeT = std::size_t;
        using StringT = Toolset::StringT;
        using StringViewT = Toolset::StringViewT;

    protected:
        struct Data final
        {
            const CharT* string = nullptr;
            SizeT size = 0;
            StringPolicy policy = StringPolicy::Static;
        };

    public:
        [[nodiscard]] constexpr const CharT* CStr() const noexcept { return GetData().string; }
        [[nodiscard]] constexpr SizeT Size() const noexcept { return GetData().size; }
        [[nodiscard]] constexpr SizeT Length() const noexcept { return GetData().size == 0; }
        [[nodiscard]] constexpr bool IsEmpty() const noexcept { return GetData().size == 0; }
        [[nodiscard]] constexpr explicit operator const CharT*() const noexcept { return GetData().string; }
        [[nodiscard]] constexpr CharT operator[](std::size_t index) const { return GetData().string[index]; }

        [[nodiscard]] virtual constexpr bool operator==(const IString& other) const { return GetData().string == other.GetData().string; }
        [[nodiscard]] virtual constexpr bool operator!=(const IString& other) const { return GetData().string != other.GetData().string; }
        [[nodiscard]] virtual constexpr bool operator==(const CharT* other) const
        {
            return Toolset::Cmp(GetData().string, other) == Comparison::Equal;
        }
        [[nodiscard]] virtual constexpr bool operator!=(const CharT* other) const
        {
            return Toolset::Cmp(GetData().string, other) != Comparison::Equal;
        }
        [[nodiscard]] virtual bool operator>(const CharT* other) const { return Toolset::Cmp(GetData().string, other) == Comparison::Greater; }
        [[nodiscard]] virtual bool operator>=(const CharT* other) const
        {
            const auto result = Toolset::Cmp(GetData().string, other);
            return result == Comparison::Greater || result == Comparison::Equal;
        }
        [[nodiscard]] virtual bool operator<(const CharT* other) const { return Toolset::Cmp(GetData().string, other) == Comparison::Less; }
        [[nodiscard]] virtual bool operator<=(const CharT* other) const
        {
            const auto result = Toolset::Cmp(GetData().string, other);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] virtual constexpr bool operator==(StringViewT other) const { return *this == other.data(); }
        [[nodiscard]] virtual bool operator>(StringViewT other) const { return *this == other.data(); }
        [[nodiscard]] virtual bool operator>=(StringViewT other) const { return *this >= other.data(); }
        [[nodiscard]] virtual bool operator<(StringViewT other) const { return *this < other.data(); }
        [[nodiscard]] virtual bool operator<=(StringViewT other) const { return *this <= other.data(); }

        [[nodiscard]] bool operator==(const StringT& other) const { return *this == other.data(); }
        [[nodiscard]] bool operator>(const StringT& other) const { return *this > other.data(); }
        [[nodiscard]] bool operator>=(const StringT& other) const { return *this >= other.data(); }
        [[nodiscard]] bool operator<(const StringT& other) const { return *this < other.data(); }
        [[nodiscard]] bool operator<=(const StringT& other) const { return *this <= other.data(); }

        [[nodiscard]] constexpr bool operator!() const noexcept { return !GetData().string; }
        [[nodiscard]] constexpr operator bool() const noexcept { return GetData().string; }

        [[nodiscard]] virtual constexpr CharT Front() const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get null string");
                return {};
            }

            return data.string[0];
        }

        [[nodiscard]] virtual constexpr CharT Back() const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get null string");
                return {};
            }

            return data.string[data.size - static_cast<decltype(data.size)>(1)];
        }

        [[nodiscard]] constexpr StringViewT ToStringView() const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get null string");
                return {};
            }

            return { data.string, data.size };
        }

        [[nodiscard]] constexpr StringT ToStdString() const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get null string");
                return {};
            }

            return { data.string };
        }

        [[nodiscard]] virtual CharT At(SizeT index) const noexcept
        {
            Data data = GetData();
            if (!data.string || data.size >= index)
            {
                Assert("Was get null string");
                return {};
            }

            return data.string[index];
        }

        [[nodiscard]] std::vector<StringT> Split(CharT delimiter) const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get null string");
                return {};
            }

            constexpr SizeT defaultSize = 16ull;

            std::vector<StringT> splittedStrings;

            StringT temp;
            temp.resize(defaultSize);
            while (data.string)
            {
            }

            return splittedStrings;
        }

        template<class T>
        [[nodiscard]] std::optional<T> ConvertTo() const noexcept
        {
            static_assert(false, "Invalid type");
        }

        template<>
        [[nodiscard]] std::optional<int> ConvertTo() const noexcept
        {
            return { Toolset::ToInt(GetData().string) };
        }

        template<>
        [[nodiscard]] std::optional<float> ConvertTo() const noexcept
        {
            return { Toolset::ToFloat(GetData().string) };
        }

        template<>
        [[nodiscard]] std::optional<long long> ConvertTo() const noexcept
        {
            return { Toolset::ToLongLong(GetData().string) };
        }

        // [[nodiscard]] virtual std::vector<StringT> Split(StringViewT delimiter) const = 0;

        // [[nodiscard]] virtual StringT TrimStart(StringViewT value) const = 0;
        // [[nodiscard]] virtual StringT TrimEnd(StringViewT value) const = 0;
        // [[nodiscard]] StringT Trim(StringViewT value) const { TrimStart(value); TrimEnd(value); }

        // [[nodiscard]] virtual StringT ToUpperCase() const = 0;
        // [[nodiscard]] virtual StringT ToLowerCase() const = 0;

        // [[nodiscard]] virtual StringT Replace(StringViewT oldValue, StringViewT newValue) const = 0;
        // [[nodiscard]] virtual StringT ReplaceAll(StringViewT oldValue, StringViewT newValue) const = 0;

        // [[nodiscard]] virtual std::size_t Find(StringViewT value) const = 0;
        // [[nodiscard]] virtual std::size_t RFind(StringViewT value) const = 0;
        // [[nodiscard]] virtual std::size_t Find(const std::regex& expr) const = 0;
        // [[nodiscard]] virtual std::size_t RFind(const std::regex& expr) const = 0;

    protected:
        [[nodiscard]] virtual constexpr Data GetData() const = 0;
    };

    template<class CharType, StringPolicy PolicyValue>
    class BaseStringWrapper;

    template<class CharType, StringPolicy PolicyValue>
    class BaseString
    {
        static_assert("Invalid string");
    };

    template<class CharType>
    class BaseString<CharType, StringPolicy::Static> : public IString<CharType>
    {
    public:
        constexpr static StringPolicy Policy = StringPolicy::Static;
        using Self = BaseString<CharType, Policy>;
        using Parent = IString<CharType>;
        using CharT = CharType;
        using Toolset = Parent::Toolset;
        using SizeT = Parent::SizeT;
        using StringT = Parent::StringT;
        using StringViewT = Parent::StringViewT;
        using WrapperT = BaseStringWrapper<CharT, Policy>;

    public:
        constexpr BaseString(const WrapperT& wrapper)
            : _string(wrapper._string),
              _size(wrapper._size)
        {
        }

    protected:
        [[nodiscard]] constexpr Parent::Data GetData() const override { return { _string, _size, Policy }; }

    private:
        explicit constexpr BaseString(const CharT* string, SizeT size)
            : _string{ string },
              _size{ size }
        {
        }

        const CharT* _string = nullptr;
        const SizeT _size = 0;
    };

    template<class CharType>
    class BaseString<CharType, StringPolicy::Dynamic> : public IString<CharType>
    {
    public:
        constexpr static StringPolicy Policy = StringPolicy::Dynamic;
        using Self = BaseString<CharType, Policy>;
        using Parent = IString<CharType>;
        using CharT = CharType;
        using Toolset = Parent::Toolset;
        using SizeT = Parent::SizeT;
        using StringT = Parent::StringT;
        using StringViewT = Parent::StringViewT;
        using StaticWrapperT = BaseStringWrapper<CharT, StringPolicy::Static>;

    public:
        constexpr BaseString(const StaticWrapperT& wrapper) { static_assert(false); }

        BaseString(const CharT* string)
        {
            Assert(string);
            if (string)
            {
                static_assert(false);
            }
        }

    protected:
        [[nodiscard]] constexpr Parent::Data GetData() const override { return { _string, _size, Policy }; }

    private:
        explicit constexpr BaseString(const CharT* string, SizeT size)
            : _string{ string },
              _size{ size }
        {
        }

        CharT* _string = nullptr;
        SizeT _size = 0;
    };

    template<class CharType, StringPolicy PolicyValue>
    class BaseStringWrapper : Utils::NotCopyableAndNotMoveable
    {
    public:
        using StringT = BaseString<CharType, PolicyValue>;
        constexpr explicit BaseStringWrapper(const StringT::CharT* str, StringT::SizeT size)
            : _string{ str },
              _size{ size }
        {
        }

    private:
        const StringT::CharT* _string = nullptr;
        StringT::SizeT _size = 0;

        friend class BaseString<CharType, PolicyValue>;
    };

    using StringAtom = BaseString<char, StringPolicy::Static>;
    using String = BaseString<char, StringPolicy::Dynamic>;

} // namespace Core

constexpr Core::BaseStringWrapper<char, Core::StringPolicy::Static> operator""_atom(const char* str, std::size_t size) noexcept
{
    return Core::BaseStringWrapper<char, Core::StringPolicy::Static>(str, size);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator>(const CharType* str1, const Core::BaseString<CharType, StringPolicy>& str2)
{
    return !(str2 > str1);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator>=(const CharType* str1, const Core::BaseString<CharType, StringPolicy>& str2)
{
    return !(str2 >= str1);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator<(const CharType* str1, const Core::BaseString<CharType, StringPolicy>& str2)
{
    return !(str2 < str1);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator<=(const CharType* str1, const Core::BaseString<CharType, StringPolicy>& str2)
{
    return !(str2 <= str1);
}
