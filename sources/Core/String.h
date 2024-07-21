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

#include "Core/AbstractIterators.h"
#include "Core/Assert.h"
#include "Core/CommonEnums.h"
#include "Singleton.h"
#include "Utils/CopyableAndMoveableBehaviour.h"

#include <cstring>
#include <optional>
#include <regex>
#include <type_traits>
#include <unordered_map>
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
    struct _StringSettings : public Utils::Abstract
    {
        using CharT = CharType;
        using SizeT = std::size_t;
        constexpr static SizeT invalidSize = ~static_cast<SizeT>(0);
    };

    template<class CharType>
    struct StringDataReadOnly
    {
        using Settings = _StringSettings<CharType>;

        typename Settings::CharT* str;
        typename Settings::SizeT size = Settings::invalidSize;
    };

    template<class CharType>
    struct StringData
    {
        using Settings = _StringSettings<CharType>;
        using SmartPointer = std::unique_ptr<typename Settings::CharT[]>;

        [[nodiscard]] StringDataReadOnly<CharType> ToReadOnly() noexcept { return StringDataReadOnly<CharType>{ str.get(), size }; }

        SmartPointer str;
        typename Settings::SizeT size = Settings::invalidSize;
        std::size_t copyCounts = 0;
    };

    template<class CharType>
    struct _StringToolset;

    template<>
    struct _StringToolset<char> : public Utils::Abstract
    {
        using CharT = char;
        using StringT = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StringViewT = std::basic_string_view<CharT>;

        [[nodiscard]] static std::size_t Length(const CharT* string) noexcept { return static_cast<std::size_t>(strlen(string)); }
        [[nodiscard]] static int ToInt(const CharT* str) noexcept { return atoi(str); }
        [[nodiscard]] static float ToFloat(const CharT* str) noexcept { return static_cast<float>(atof(str)); }
        [[nodiscard]] static double ToDouble(const CharT* str) noexcept { return atof(str); }
        [[nodiscard]] static long long ToLongLong(const CharT* str) noexcept { return atof(str); }
        [[nodiscard]] static CharT* StrTok(CharT* string, const CharT* delim, CharT*& context) noexcept { return strtok_s(string, delim, &context); };
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
        [[nodiscard]] static CharT* StrTok(CharT* string, const CharT* delim, CharT*& context) noexcept { return wcstok_s(string, delim, &context); };
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
    class _StringPool : public Singleton<_StringPool<CharType>, Utils::NotCopyableAndNotMoveable>
    {
    public:
        using CharT = CharType;
        using Toolset = _StringToolset<CharT>;
        using HashT = std::size_t;
        using Settings = _StringSettings<CharT>;
        using StringDataT = StringData<CharT>;
        using StringDataReadOnlyT = StringDataReadOnly<CharT>;

    public:
        [[nodiscard]] StringDataReadOnlyT Add(const CharT* string, typename Settings::SizeT size)
        {
            const auto currentHash = std::hash<std::string_view>{}({ string, size });
            if (auto&& it = _strings.find(currentHash); it != _strings.end())
            {
                return it->second.ToReadOnly();
            }

            auto&& ptr = std::make_unique<CharT[]>(size + static_cast<typename Settings::SizeT>(1));
            memcpy(ptr.get(), string, size);
            auto* addr = ptr.get();
            _strings.emplace(currentHash, StringDataT{ std::move(ptr), size, 1 });

            return StringDataReadOnlyT{ addr, size };
        }

    private:
        std::unordered_map<HashT, StringDataT> _strings;
    };

    class Iterator;

    template<class CharType>
    class BaseString : public Utils::CopyableAndMoveable
    {
    public:
        using CharT = CharType;
        using Self = BaseString<CharT>;
        using Toolset = _StringToolset<CharT>;
        using Settings = _StringSettings<CharT>;
        using SizeT = typename Settings::SizeT;
        using StringT = typename Toolset::StringT;
        using StringViewT = typename Toolset::StringViewT;
        using StringDataReadOnlyT = StringDataReadOnly<CharT>;
        using StringPool = _StringPool<CharT>;

    public:

        template<bool IsReversed>
        class Iterator : public IRandomAccessIterator<CharT, Iterator<IsReversed>, Utils::CopyableAndMoveable, true>
        {
        public:
            using Self = Iterator;
            using Super = IRandomAccessIterator<CharT, Iterator, Utils::CopyableAndMoveable, true>;

        public:
            Iterator() = default;

            [[nodiscard]] const Super::DataRefT operator*() const noexcept override { return *_data; }

            [[nodiscard]] const Super::DataRefT operator->() const override { return *_data; }

            [[nodiscard]] Super::DataRefT operator*() noexcept override { return *_data; }

            [[nodiscard]] Super::DataRefT operator->() noexcept override { return *_data; }

            Self& operator++() noexcept override
            {
                _data += (IsReversed ? -1 : 1);
                return *this;
            }

            Self operator++(int) noexcept override
            {
                auto temp = *this;
                _data += (IsReversed ? -1 : 1);
                return temp;
            }

            Self& operator--() noexcept override
            {
                _data -= (IsReversed ? -1 : 1);
                return *this;
            }

            Self operator--(int) noexcept override
            {
                auto temp = *this;
                _data -= (IsReversed ? -1 : 1);
                return temp;
            }

            Self& operator+=(int step) noexcept override
            {
                _data += (IsReversed ? -step : step);
                return *this;
            }

            Self& operator-=(int step) noexcept override
            {
                _data -= step;
                return *this;
            }

            Self operator+(int step) const noexcept override { return Self{ _data + (IsReversed ? -step : step) }; }

            Self operator-(int step) const noexcept override { return Self{ _data - (IsReversed ? -step : step) }; }

            [[nodiscard]] bool operator>(const Self& other) const noexcept override { return (*this <=> other) == Comparison::Greater; }

            [[nodiscard]] bool operator>=(const Self& other) const noexcept override
            {
                const auto result = *this <=> other;
                return result == Comparison::Equal || result == Comparison::Greater;
            }

            [[nodiscard]] bool operator<(const Self& other) const noexcept override { return (*this <=> other) == Comparison::Less; }

            [[nodiscard]] bool operator<=(const Self& other) const noexcept override
            {
                const auto result = *this <=> other;
                return result == Comparison::Equal || result == Comparison::Less;
            }

            void Swap(Self& other) override
            {
                auto temp = *this;
                *this = other;
                other = temp;
            }

        protected:
            explicit Iterator(CharT* data)
                : _data{ data }
            {
            }

            [[nodiscard]] Comparison operator<=>(const Self& other) const noexcept
            {
                if (!_data || !other._data)
                {
                    Assert(false, "Impossible to compare two iterators. Some iterator is invalid");
                    return Comparison::None;
                }

                if (*_data > *other._data)
                {
                    return Comparison::Greater;
                }

                if (*_data == *other._data)
                {
                    return Comparison::Equal;
                }

                if (*_data < *other._data)
                {
                    return Comparison::Less;
                }

                Assert(false, "Impossible to compare two iterators. Was get some error");
                return Comparison::None;
            }

        private:
            CharT* _data = nullptr;

            friend class BaseString<CharType>;
        };

        using IteratorT = Iterator<false>;
        using ReverseIteratorT = Iterator<true>;

    public:
        [[nodiscard]] IteratorT begin() noexcept { return IteratorT{ _string }; }
        [[nodiscard]] const IteratorT begin() const noexcept { return IteratorT{ _string }; }
        [[nodiscard]] const IteratorT cbegin() const noexcept { return IteratorT{ _string }; }
        [[nodiscard]] IteratorT end() noexcept { return IteratorT{ _string + _size }; }
        [[nodiscard]] const IteratorT end() const noexcept { return IteratorT{ _string + _size }; }
        [[nodiscard]] const IteratorT cend() const noexcept { return IteratorT{ _string + _size }; }

        [[nodiscard]] ReverseIteratorT rbegin() noexcept { return ReverseIteratorT{ _string }; }
        [[nodiscard]] const ReverseIteratorT rbegin() const noexcept { return ReverseIteratorT{ _string }; }
        [[nodiscard]] const ReverseIteratorT crbegin() const noexcept { return ReverseIteratorT{ _string }; }
        [[nodiscard]] ReverseIteratorT rend() noexcept { return ReverseIteratorT{ _string + _size }; }
        [[nodiscard]] const ReverseIteratorT rend() const noexcept { return ReverseIteratorT{ _string + _size }; }
        [[nodiscard]] const ReverseIteratorT crend() const noexcept { return ReverseIteratorT{ _string + _size }; }


        [[nodiscard]] static Self Intern(const CharT* newString, SizeT size = Settings::invalidSize)
        {
            return Self{ StringPool::Instance().Add(newString, size == Settings::invalidSize ? Toolset::Length(newString) : size) };
        }
        [[nodiscard]] static Self Intern(const StringT& string) { return Self{ StringPool::Instance().Add(string.data(), string.size()) }; }

        [[nodiscard]] constexpr const CharT* CStr() const noexcept { return _string; }
        [[nodiscard]] constexpr SizeT Size() const noexcept { return _size; }
        [[nodiscard]] constexpr SizeT Length() const noexcept { return _size == 0; }
        [[nodiscard]] constexpr bool IsEmpty() const noexcept { return _size == 0; }
        [[nodiscard]] constexpr explicit operator const CharT*() const noexcept { return _string; }
        [[nodiscard]] constexpr CharT operator[](std::size_t index) const { return _string[index]; }

        [[nodiscard]] constexpr bool operator==(const Self& other) const { return _string == other._string; }
        [[nodiscard]] constexpr bool operator!=(const Self& other) const { return _string != other._string; }
        [[nodiscard]] bool operator>(const Self& other) const { return Toolset::Cmp(_string, other._string) == Comparison::Greater; }
        [[nodiscard]] bool operator>=(const Self& other) const
        {
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Greater || result == Comparison::Equal;
        }
        [[nodiscard]] bool operator<(const Self& other) const { return Toolset::Cmp(_string, other._string) == Comparison::Less; }
        [[nodiscard]] bool operator<=(const Self& other) const
        {
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const CharT* other) const { return Toolset::Cmp(_string, other) == Comparison::Equal; }
        [[nodiscard]] bool operator!=(const CharT* other) const { return Toolset::Cmp(_string, other) != Comparison::Equal; }
        [[nodiscard]] bool operator>(const CharT* other) const { return Toolset::Cmp(_string, other) == Comparison::Greater; }
        [[nodiscard]] bool operator>=(const CharT* other) const
        {
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Greater || result == Comparison::Equal;
        }
        [[nodiscard]] bool operator<(const CharT* other) const { return Toolset::Cmp(_string, other) == Comparison::Less; }
        [[nodiscard]] bool operator<=(const CharT* other) const
        {
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const StringT& other) const { return *this == other.data(); }
        [[nodiscard]] bool operator>(const StringT& other) const { return *this > other.data(); }
        [[nodiscard]] bool operator>=(const StringT& other) const { return *this >= other.data(); }
        [[nodiscard]] bool operator<(const StringT& other) const { return *this < other.data(); }
        [[nodiscard]] bool operator<=(const StringT& other) const { return *this <= other.data(); }

        [[nodiscard]] constexpr bool operator!() const noexcept { return !_string; }
        [[nodiscard]] constexpr operator bool() const noexcept { return _string; }

        [[nodiscard]] constexpr CharT Front() const
        {
            if (!_string)
            {
                Assert("Was get a null string");
                return {};
            }

            return _string[0];
        }

        [[nodiscard]] constexpr CharT Back() const
        {
            if (!_string)
            {
                Assert("Was get a null string");
                return {};
            }

            return _string[_size - static_cast<decltype(_size)>(1)];
        }

        [[nodiscard]] constexpr StringViewT ToStringView() const
        {
            if (!_string)
            {
                Assert("Was get a null string");
                return {};
            }

            return { _string, _size };
        }

        [[nodiscard]] constexpr StringT ToStdString() const
        {
            if (!_string)
            {
                Assert("Was get a null string");
                return {};
            }

            return { _string };
        }

        [[nodiscard]] virtual CharT At(SizeT index) const noexcept
        {
            if (!_string || _size >= index)
            {
                Assert("Was get a null string");
                return {};
            }

            return _string[index];
        }

        [[nodiscard]] std::vector<StringT> Split(const StringT& delimiter) const
        {
            StringT string;
            {
                if (!_string)
                {
                    Assert("Was get a null string");
                    return {};
                }
                string = _string;
            }

            std::vector<StringT> splittedStrings;
            CharT* context = nullptr;

            if (const auto* token = Toolset::StrTok(string.data(), delimiter.c_str(), context))
            {
                splittedStrings.emplace_back(StringT{ token });
            }
            else
            {
                return {};
            }

            do
            {
                if (const auto* token = Toolset::StrTok(context, delimiter.c_str(), context))
                {
                    splittedStrings.emplace_back(StringT{ token });
                }
                else
                {
                    break;
                }
            } while (context);

            return splittedStrings;
        }

        template<class T>
        [[nodiscard]] T ConvertTo() const noexcept
        {
            static_assert(false, "Invalid template type was passed");
            return {};
        }

        template<>
        [[nodiscard]] int ConvertTo() const noexcept
        {
            return Toolset::ToInt(_string);
        }

        template<>
        [[nodiscard]] float ConvertTo() const noexcept
        {
            return Toolset::ToFloat(_string);
        }

        template<>
        [[nodiscard]] long long ConvertTo() const noexcept
        {
            return Toolset::ToLongLong(_string);
        }

        [[nodiscard]] std::size_t MakeHash() const noexcept { return std::hash<std::string_view>{}({ _string, _size }); }

        Self& TrimStart(StringViewT value) noexcept
        {
            if (value)
            {
            }

            return *this;
        }
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
        constexpr explicit BaseString(StringDataReadOnlyT data)
            : _string{ data.str },
              _size{ data.size }
        {
        }

    protected:
        CharT* _string = nullptr;
        SizeT _size = 0;
        StringPolicy _policy = StringPolicy::Static;
    };

    using StringAtom = BaseString<char>;

} // namespace Core

namespace std
{

    template<class CharType>
    struct hash<Core::BaseString<CharType>>
    {
        size_t operator()(const Core::BaseString<CharType>& x) const noexcept { return x.MakeHash(); }
    };

} // namespace std

inline Core::BaseString<char> operator""_atom(const char* str, std::size_t size) noexcept
{
    return Core::BaseString<char>::Intern(str, size);
}

template<class CharType>
[[nodiscard]] bool operator>(const CharType* str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1);
}

template<class CharType>
[[nodiscard]] bool operator>=(const CharType* str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1) || (str1 == str2);
}

template<class CharType>
[[nodiscard]] bool operator<(const CharType* str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1);
}

template<class CharType>
[[nodiscard]] bool operator<=(const CharType* str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1) || (str1 == str2);
}

template<class CharType>
[[nodiscard]] bool operator>(const std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>& str1,
                             const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1);
}

template<class CharType>
[[nodiscard]] bool operator>=(const std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>& str1,
                              const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1) || (str1 == str2);
}

template<class CharType>
[[nodiscard]] bool operator<(const std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>& str1,
                             const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1);
}

template<class CharType>
[[nodiscard]] bool operator<=(const std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>& str1,
                              const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1) || (str1 == str2);
}
