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
#include "Singleton.h"
#include "Utils/CopyableAndMoveableBehaviour.h"

#include <optional>
#include <regex>
#include <string_view>
#include <type_traits>
#include <unordered_set>
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
        using StringT = typename std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StringViewT = typename std::basic_string_view<CharT>;

        [[nodiscard]] static std::size_t Length(const CharT* string) noexcept { return static_cast<std::size_t>(strlen(string)); }
        [[nodiscard]] static int ToInt(const CharT* str) noexcept { return atoi(str); }
        [[nodiscard]] static float ToFloat(const CharT* str) noexcept { return static_cast<float>(atof(str)); }
        [[nodiscard]] static double ToDouble(const CharT* str) noexcept { return atof(str); }
        [[nodiscard]] static long long ToLongLong(const CharT* str) noexcept { return atof(str); }

        [[nodiscard]] static Comparison Cmp(const CharT* str1, const CharT* str2) noexcept
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
        using StringT = typename std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StringViewT = typename std::basic_string_view<CharT>;

        [[nodiscard]] static std::size_t Length(const CharT* string) noexcept { return static_cast<std::size_t>(wcslen(string)); }
        [[nodiscard]] static int ToInt(const CharT* str) noexcept { return _wtoi(str); }
        [[nodiscard]] static float ToFloat(const CharT* str) noexcept { return static_cast<float>(_wtof(str)); }
        [[nodiscard]] static double ToDouble(const CharT* str) noexcept { return _wtof(str); }
        [[nodiscard]] static long long ToLongLong(const CharT* str) noexcept { return _wtoll(str); }

        [[nodiscard]] static Comparison Cmp(const CharT* str1, const CharT* str2) noexcept
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
        using CharT = typename CharType;
        using Toolset = typename _StringToolset<CharT>;
        using SizeT = typename std::size_t;
        using StringT = typename Toolset::StringT;
        using StringViewT = typename Toolset::StringViewT;

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
                Assert("Was get a null string");
                return {};
            }

            return data.string[0];
        }

        [[nodiscard]] virtual constexpr CharT Back() const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get a null string");
                return {};
            }

            return data.string[data.size - static_cast<decltype(data.size)>(1)];
        }

        [[nodiscard]] constexpr StringViewT ToStringView() const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get a null string");
                return {};
            }

            return { data.string, data.size };
        }

        [[nodiscard]] constexpr StringT ToStdString() const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get a null string");
                return {};
            }

            return { data.string };
        }

        [[nodiscard]] virtual CharT At(SizeT index) const noexcept
        {
            Data data = GetData();
            if (!data.string || data.size >= index)
            {
                Assert("Was get a null string");
                return {};
            }

            return data.string[index];
        }

        [[nodiscard]] std::vector<StringT> Split(CharT delimiter) const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get a null string");
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

    template<class CharType>
    class _StringPool : public Singleton<_StringPool<CharType>, Utils::NotCopyableAndNotMoveable>
    {
    public:
        using CharT = typename CharType;
        using SizeT = typename std::size_t; // TODO: fix it, so BasicString has another 'using' - use it
        using Toolset = typename _StringToolset<CharT>;
        using WrapperT = typename BaseStringWrapper<CharT, StringPolicy::Static>;
        using SmartPointer = typename std::unique_ptr<CharT[]>;

    public:
        [[nodiscard]] WrapperT Add(const CharT* string, SizeT size)
        {
            auto&& ptr = std::make_unique<CharT[]>(size + static_cast<SizeT>(1));
            memcpy(ptr.get(), string, size);
            const auto* addr = ptr.get();
            _strings.emplace(std::move(ptr));

            return WrapperT{ addr, size };
        }

    private:
        std::unordered_set<SmartPointer> _strings;
    };

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
        using Self = typename BaseString<CharType, Policy>;
        using Parent = typename IString<CharType>;
        using CharT = typename CharType;
        using Toolset = typename Parent::Toolset;
        using SizeT = typename Parent::SizeT;
        using StringT = typename Parent::StringT;
        using StringViewT = typename Parent::StringViewT;
        using WrapperT = typename BaseStringWrapper<CharT, Policy>;
        using StringPool = typename _StringPool<CharT>;

    public:
        /**
         * @brief Don't use this constructor manually
         */
        constexpr BaseString(const WrapperT& wrapper)
            : _string(wrapper._string),
              _size(wrapper._size)
        {
        }

        [[nodiscard]] static WrapperT Intern(const CharT* newString) { return StringPool::Instance().Add(newString, Toolset::Length(newString)); }

        [[nodiscard]] static WrapperT Intern(const StringT& newString) { return StringPool::Instance().Add(newString.c_str(), newString.size()); }

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
