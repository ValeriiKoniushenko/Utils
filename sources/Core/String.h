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
        None,
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
        [[nodiscard]] static long long ToLongLong(const CharT* str) noexcept { return atol(str); }
        [[nodiscard]] static CharT* StrTok(CharT* string, const CharT* delim, CharT*& context) noexcept { return strtok_s(string, delim, &context); };
        [[nodiscard]] static CharT* StrStr(CharT* mainString, const CharT* subString) noexcept { return strstr(mainString, subString); };
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
        using StringT = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StringViewT = std::basic_string_view<CharT>;

        [[nodiscard]] static std::size_t Length(const CharT* string) noexcept { return static_cast<std::size_t>(wcslen(string)); }
        [[nodiscard]] static int ToInt(const CharT* str) noexcept { return _wtoi(str); }
        [[nodiscard]] static float ToFloat(const CharT* str) noexcept { return static_cast<float>(_wtof(str)); }
        [[nodiscard]] static double ToDouble(const CharT* str) noexcept { return _wtof(str); }
        [[nodiscard]] static CharT* StrTok(CharT* string, const CharT* delim, CharT*& context) noexcept { return wcstok_s(string, delim, &context); };
        [[nodiscard]] static long long ToLongLong(const CharT* str) noexcept { return _wtoll(str); }
        [[nodiscard]] static CharT* StrStr(CharT* mainString, const CharT* subString) noexcept { return wcsstr(mainString, subString); };
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
                ++it->second.copyCounts;
                return it->second.ToReadOnly();
            }

            auto&& ptr = std::make_unique<CharT[]>(size + static_cast<typename Settings::SizeT>(1));
            memcpy(ptr.get(), string, size * sizeof(CharT));
            auto* addr = ptr.get();
            _strings.emplace(currentHash, StringDataT{ std::move(ptr), size, 1 });

            return StringDataReadOnlyT{ addr, size };
        }

        void Remove(CharT*& string, typename Settings::SizeT size)
        {
            const auto currentHash = std::hash<std::string_view>{}({ string, size });
            if (auto&& it = _strings.find(currentHash); it != _strings.end())
            {
                --it->second.copyCounts;
                if (it->second.copyCounts == 0)
                {
                    _strings.erase(it);
                }
                string = nullptr;
            }
            else
            {
                Assert(false, "Impossible to find such string");
            }
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

            [[nodiscard]] bool operator==(const Self& other) const noexcept override { return _data == other._data; };

            [[nodiscard]] bool operator!=(const Self& other) const noexcept override { return _data != other._data; };

            [[nodiscard]] const typename Super::DataRefT operator*() const noexcept override { return *_data; }

            [[nodiscard]] const typename Super::DataRefT operator->() const override { return *_data; }

            [[nodiscard]] typename Super::DataRefT operator*() noexcept override { return *_data; }

            [[nodiscard]] typename Super::DataRefT operator->() noexcept override { return *_data; }

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

        [[nodiscard]] ReverseIteratorT rbegin() noexcept { return ReverseIteratorT{ _string + _size }; }
        [[nodiscard]] const ReverseIteratorT rbegin() const noexcept { return ReverseIteratorT{ _string + _size }; }
        [[nodiscard]] const ReverseIteratorT crbegin() const noexcept { return ReverseIteratorT{ _string + _size }; }
        [[nodiscard]] ReverseIteratorT rend() noexcept { return ReverseIteratorT{ _string }; }
        [[nodiscard]] const ReverseIteratorT rend() const noexcept { return ReverseIteratorT{ _string }; }
        [[nodiscard]] const ReverseIteratorT crend() const noexcept { return ReverseIteratorT{ _string }; }

        [[nodiscard]] static Self Intern(const CharT* newString, SizeT size = Settings::invalidSize)
        {
            return Self{ StringPool::Instance().Add(newString, size == Settings::invalidSize ? Toolset::Length(newString) : size) };
        }
        [[nodiscard]] static Self Intern(const StringT& string) { return Self{ StringPool::Instance().Add(string.data(), string.size()) }; }

        [[nodiscard]] constexpr const CharT* CStr() const noexcept { return _string; }
        [[nodiscard]] constexpr SizeT Size() const noexcept { return _size; }
        [[nodiscard]] constexpr SizeT Length() const noexcept { return _size; }
        [[nodiscard]] constexpr bool IsEmpty() const noexcept { return _string == nullptr || _size == 0; }
        [[nodiscard]] constexpr explicit operator const CharT*() const noexcept { return _string; }
        [[nodiscard]] constexpr CharT operator[](std::size_t index) const noexcept { return _string[index]; }

        [[nodiscard]] constexpr bool operator==(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return IsStatic() && other.IsStatic() ? _string == other._string : Toolset::Cmp(_string, other._string) == Comparison::Equal;
        }
        [[nodiscard]] constexpr bool operator!=(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return IsStatic() && other.IsStatic() ? _string != other._string : Toolset::Cmp(_string, other._string) != Comparison::Equal;
        }
        [[nodiscard]] bool operator>(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other._string) == Comparison::Greater;
        }
        [[nodiscard]] bool operator>=(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Greater || result == Comparison::Equal;
        }
        [[nodiscard]] bool operator<(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other._string) == Comparison::Less;
        }
        [[nodiscard]] bool operator<=(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Equal;
        }
        [[nodiscard]] bool operator!=(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) != Comparison::Equal;
        }
        [[nodiscard]] bool operator>(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Greater;
        }
        [[nodiscard]] bool operator>=(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Greater || result == Comparison::Equal;
        }
        [[nodiscard]] bool operator<(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Less;
        }
        [[nodiscard]] bool operator<=(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const StringT& other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return *this == other.data();
        }
        [[nodiscard]] bool operator>(const StringT& other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return *this > other.data();
        }
        [[nodiscard]] bool operator>=(const StringT& other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return *this >= other.data();
        }
        [[nodiscard]] bool operator<(const StringT& other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return *this < other.data();
        }
        [[nodiscard]] bool operator<=(const StringT& other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }
            return *this <= other.data();
        }

        [[nodiscard]] constexpr bool operator!() const noexcept { return !IsEmpty(); }
        [[nodiscard]] constexpr operator bool() const noexcept { return IsEmpty(); }

        [[nodiscard]] constexpr CharT Front() const
        {
            if (IsEmpty())
            {
                Assert("Was get a null string");
                return {};
            }

            return _string[0];
        }

        [[nodiscard]] constexpr CharT Back() const
        {
            if (IsEmpty())
            {
                Assert("Was get a null string");
                return {};
            }

            return _string[_size - static_cast<SizeT>(1)];
        }

        [[nodiscard]] constexpr StringViewT ToStringView() const
        {
            if (IsEmpty())
            {
                Assert("Was get a null string");
                return {};
            }

            return { _string, _size };
        }

        [[nodiscard]] constexpr StringT ToStdString() const
        {
            if (IsEmpty())
            {
                Assert("Was get a null string");
                return {};
            }

            return { _string };
        }

        [[nodiscard]] CharT At(SizeT index) const noexcept
        {
            if (IsEmpty() || _size >= index)
            {
                Assert("Was get a null string or invalid index");
                return {};
            }

            return _string[index];
        }

        [[nodiscard]] std::vector<StringT> Split(const StringT& delimiter) const
        {
            if (IsEmpty())
            {
                Assert(false, "Impossible to work with nullptr string.");
                return {};
            }

            StringT string = _string;

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
        [[nodiscard]] T ConvertTo() const noexcept;

        template<>
        [[nodiscard]] int ConvertTo() const noexcept
        {
            if (!IsEmpty())
            {
                return Toolset::ToInt(_string);
            }
            Assert(false, "Impossible to work with nullptr string.");
            return {};
        }

        template<>
        [[nodiscard]] float ConvertTo() const noexcept
        {
            if (!IsEmpty())
            {
                return Toolset::ToFloat(_string);
            }
            Assert(false, "Impossible to work with nullptr string.");
            return {};

        }

        template<>
        [[nodiscard]] long long ConvertTo() const noexcept
        {
            if (!IsEmpty())
            {
                return Toolset::ToLongLong(_string);
            }
            Assert(false, "Impossible to work with nullptr string.");
            return {};
        }

        [[nodiscard]] std::size_t MakeHash() const noexcept
        {
            if (!IsEmpty())
            {
                return std::hash<std::string_view>{}({ _string, _size });
            }
            Assert(false, "Impossible to make a hash from nullptr string.");
            return {};
        }

        Self& SubStr(std::size_t index, SizeT count = 0) noexcept
        {
            if (!IsEmpty())
            {
                const SizeT finalCount = count == 0 ? _size - index : count;
                TryToMakeAsDynamicFrom(_string + index, _string + index + finalCount);
            }

            return *this;
        }

        Self& TrimStart(StringViewT value) noexcept
        {
            if (!IsEmpty())
            {
                if (value)
                {
                    TryToMakeAsDynamic();
                    if (const CharT* str = Toolset::StrStr(_string, value.data()))
                    {

                    }
                }
            }

            return *this;
        }

        [[nodiscard]] constexpr bool IsStatic() const noexcept { return _policy == StringPolicy::Static; }
        [[nodiscard]] constexpr bool IsDynamic() const noexcept { return _policy == StringPolicy::Dynamic; }
        [[nodiscard]] constexpr bool CheckFor(StringPolicy policy) const noexcept { return _policy == policy; }

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

        BaseString(const BaseString& other)
        {
            *this = other;
        }

        BaseString& operator=(const BaseString& other)
        {
            if (this == &this)
            {
                return *this;
            }

            if (other._policy == StringPolicy::Dynamic)
            {
                Clear();
                TryToMakeAsDynamicFrom(other._string, other._string + other._size);
            }
            else if (other._policy == StringPolicy::Static)
            {
                Clear();
                auto&& data = StringPool::Instance().Add(other._string, other._size);
                _policy = StringPolicy::Static;
                _string = data.str;
                _size = data.size;
            }
            else
            {
                Assert(false, "Invalid StringPolicy type. Impossible to copy\\assign.");
            }

            return *this;
        }

        BaseString(BaseString&& other) noexcept
        {
            *this = std::move(other);
        }

        BaseString& operator=(BaseString&& other) noexcept
        {
            if (other._policy == StringPolicy::Dynamic)
            {
                Clear();
                _string = other._string;
                _size = other._size;
                _policy = StringPolicy::Dynamic;

                other._size = 0;
                other._string = nullptr;
                other._policy = StringPolicy::None;
            }
            else if (other._policy == StringPolicy::Static)
            {
                Clear();
                _policy = StringPolicy::Static;
                _string = other._string;
                _size = other._size;

                other._size = 0;
                other._string = nullptr;
                other._policy = StringPolicy::None;
            }
            else
            {
                Assert(false, "Invalid StringPolicy type. Impossible to copy\\assign.");
            }

            return *this;
        }

        void Clear()
        {
            if (_string)
            {
                if (_policy == StringPolicy::Static)
                {
                    StringPool::Instance().Remove(_string, _size);
                    _size = 0;
                    _policy = StringPolicy::None;
                }
                else if(_policy == StringPolicy::Dynamic)
                {
                    delete[] _string;
                    _string = nullptr;
                    _size = 0;
                    _policy = StringPolicy::None;
                }
                else
                {
                    Assert(false, "Invalid StringPolicy type. Impossible to delete.");
                }
            }
        }

        ~BaseString() override
        {
            Clear();
        }

    protected:
        constexpr explicit BaseString(StringDataReadOnlyT data)
            : _string{ data.str },
              _size{ data.size },
              _policy{ StringPolicy::Static }
        {
        }

        void TryToMakeAsDynamic()
        {
            if (_policy == StringPolicy::Static)
            {
                const auto* oldString = _string;
                if ((_string = new CharT[_size + static_cast<SizeT>(1)]))
                {
                    _string[_size] = 0;
                    memcpy_s(_string, _size * sizeof(CharT), oldString, _size * sizeof(CharT));
                    StringPool::Instance().Remove(oldString, _size);
                    _policy = StringPolicy::Dynamic;
                }
            }
        }

        void TryToMakeAsDynamicFrom(const CharT* begin, const CharT* end)
        {
            const SizeT finalSize = end - begin;

            if (auto* newString = new CharT[finalSize + static_cast<SizeT>(1)])
            {
                newString[finalSize] = 0;
                memcpy_s(newString, finalSize * sizeof(CharT), begin, finalSize * sizeof(CharT));

                Clear();

                _policy = StringPolicy::Dynamic;
                _string = newString;
                _size = finalSize;
            }
        }

    protected:
        CharT* _string = nullptr;
        SizeT _size = 0;
        StringPolicy _policy = StringPolicy::None;
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
