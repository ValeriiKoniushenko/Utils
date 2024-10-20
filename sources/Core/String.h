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
#include <cwctype>
#include <functional>
#include <optional>
#include <regex>
#include <set>
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
        using HashT = std::size_t;
        using IndexT = std::size_t;
        constexpr static SizeT invalidSize = ~static_cast<SizeT>(0);
    };

    template<class CharType>
    struct _StringToolset;

    template<>
    struct _StringToolset<char> : public Utils::Abstract
    {
        using CharT = char;
        using StdStringT = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StdStringViewT = std::basic_string_view<CharT, std::char_traits<CharT>>;
        using StdRegex = std::basic_regex<CharT, std::regex_traits<CharT>>;
        using SizeT = typename _StringSettings<CharT>::SizeT;

        [[nodiscard]] static bool IsSpace(int ch) { return static_cast<bool>(isspace(ch)); }
        [[nodiscard]] static SizeT Length(const CharT* string) noexcept { return static_cast<SizeT>(strlen(string)); }

        [[nodiscard]] static int ToInt(const CharT* str) noexcept { return atoi(str); }
        [[nodiscard]] static float ToFloat(const CharT* str) noexcept { return static_cast<float>(atof(str)); }
        [[nodiscard]] static double ToDouble(const CharT* str) noexcept { return atof(str); }
        [[nodiscard]] static long long ToLongLong(const CharT* str) noexcept { return atoll(str); }

        static void FromInt(int value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%d", value); errorCode < 0)
            {
                Assert("Impossible to convert 'int' value to string.");
            }
        }

        static void FromFloat(float value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%f", value); errorCode < 0)
            {
                Assert("Impossible to convert 'float' value to string.");
            }
        }

        static void FromDouble(double value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%lf", value); errorCode < 0)
            {
                Assert("Impossible to convert 'double' value to string.");
            }
        }

        static void FromUnsignedLongLong(unsigned long long value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%llu", value); errorCode < 0)
            {
                Assert("Impossible to convert 'long long' value to string.");
            }
        }

        [[nodiscard]] static CharT* StrTok(CharT* string, const CharT* delim, CharT*& context) noexcept { return strtok_s(string, delim, &context); };
        [[nodiscard]] static CharT* StrStr(CharT* mainString, const CharT* subString) noexcept { return strstr(mainString, subString); };

        [[nodiscard]] static int ToUpper(const CharT ch) noexcept { return toupper(ch); };
        [[nodiscard]] static int ToLower(const CharT ch) noexcept { return tolower(ch); };

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
        using StdStringT = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StdStringViewT = std::basic_string_view<CharT, std::char_traits<CharT>>;
        using StdRegex = std::basic_regex<CharT, std::regex_traits<CharT>>;
        using SizeT = typename _StringSettings<CharT>::SizeT;

        [[nodiscard]] static bool IsSpace(wint_t ch) { return static_cast<bool>(std::iswspace(ch)); }
        [[nodiscard]] static SizeT Length(const CharT* string) noexcept { return static_cast<SizeT>(wcslen(string)); }

        [[nodiscard]] static int ToInt(const CharT* str) noexcept { return _wtoi(str); }
        [[nodiscard]] static float ToFloat(const CharT* str) noexcept { return static_cast<float>(_wtof(str)); }
        [[nodiscard]] static double ToDouble(const CharT* str) noexcept { return _wtof(str); }
        [[nodiscard]] static long long ToLongLong(const CharT* str) noexcept { return _wtoll(str); }

        static void FromInt(int value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%d", value); errorCode < 0)
            {
                Assert("Impossible to convert 'int' value to string.");
            }
        }

        static void FromFloat(float value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%f", value); errorCode < 0)
            {
                Assert("Impossible to convert 'float' value to string.");
            }
        }

        static void FromDouble(double value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%lf", value); errorCode < 0)
            {
                Assert("Impossible to convert 'double' value to string.");
            }
        }

        static void FromUnsignedLongLong(unsigned long long value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%llu", value); errorCode < 0)
            {
                Assert("Impossible to convert 'long long' value to string.");
            }
        }

        [[nodiscard]] static CharT* StrTok(CharT* string, const CharT* delim, CharT*& context) noexcept { return wcstok_s(string, delim, &context); };
        [[nodiscard]] static CharT* StrStr(CharT* mainString, const CharT* subString) noexcept { return wcsstr(mainString, subString); };

        [[nodiscard]] static std::wint_t ToUpper(const CharT ch) noexcept { return towupper(ch); };
        [[nodiscard]] static std::wint_t ToLower(const CharT ch) noexcept { return towlower(ch); };

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
    struct StringDataReadOnly
    {
        using Settings = _StringSettings<CharType>;

        typename Settings::CharT* str;
        typename Settings::SizeT size = Settings::invalidSize;
    };

    template<class CharType>
    struct StringData
    {
        using Toolset = _StringToolset<CharType>;
        using Settings = _StringSettings<CharType>;
        using SmartPointer = std::unique_ptr<typename Settings::CharT[]>;

        StringData(SmartPointer&& ptr, typename Settings::SizeT newSize)
            : str{ std::move(ptr) },
              size{ newSize }
        {
        }

        StringData(const CharType* newString, typename Settings::SizeT newSize)
        {
            str = SmartPointer(new CharType[newSize + static_cast<decltype(newSize)>(1)]);
            memcpy_s(str.get(), size * sizeof(CharType), newString, newSize * sizeof(CharType));
        }

        [[nodiscard]] StringDataReadOnly<CharType> ToReadOnly() noexcept { return StringDataReadOnly<CharType>{ str.get(), size }; }

        [[nodiscard]] bool operator<(const StringData& other) const
        {
            if (str)
            {
                return Toolset::Cmp(str.get(), other.str.get()) == Comparison::Less;
            }
            return false;
        }

        [[nodiscard]] bool operator==(const StringData& other) const
        {
            if (str)
            {
                return Toolset::Cmp(str.get(), other.str.get()) == Comparison::Equal;
            }
            return false;
        }

        SmartPointer str;
        typename Settings::SizeT size = Settings::invalidSize;
    };

    template<class CharType>
    class _StringPool : public Singleton<_StringPool<CharType>, Utils::NotCopyableAndNotMoveable>
    {
    public:
        using CharT = CharType;
        using Toolset = _StringToolset<CharT>;
        using StdStringViewT = typename Toolset::StdStringViewT;
        using Settings = _StringSettings<CharT>;
        using HashT = typename Settings::HashT;
        using SizeT = typename Settings::SizeT;
        using StringDataT = StringData<CharT>;
        using StringDataReadOnlyT = StringDataReadOnly<CharT>;

    public:
        [[nodiscard]] StringDataReadOnlyT Add(const CharT* string, typename Settings::SizeT size, bool isCompileTime = false)
        {
            const auto currentHash = std::hash<StdStringViewT>{}({ string, size });
            if (auto&& it = _strings.find(currentHash); it != _strings.end())
            {
                return it->second.ToReadOnly();
            }

            auto&& ptr = std::make_unique<CharT[]>(size + static_cast<typename Settings::SizeT>(1));
            memcpy(ptr.get(), string, size * sizeof(CharT));
            auto* addr = ptr.get();
            _strings.emplace(currentHash, StringDataT{ std::move(ptr), size });

            return StringDataReadOnlyT{ addr, size };
        }

    private:
        std::unordered_map<HashT, StringDataT> _strings;
    };

    class Iterator;
    template<class CharType>
    class BaseString;

    template<class T>
    concept IsFormattableType =
        std::is_same_v<std::decay_t<T>, int> || std::is_same_v<std::decay_t<T>, double> || std::is_same_v<std::decay_t<T>, float> ||
        std::is_same_v<std::decay_t<T>, unsigned long long> || std::is_same_v<std::decay_t<T>, const char*> ||
        std::is_same_v<std::decay_t<T>, char*> || std::is_same_v<std::decay_t<T>, const wchar_t*> || std::is_same_v<std::decay_t<T>, wchar_t*> ||
        std::is_same_v<BaseString<T::CharT>, T>;

    template<class CharType>
    class BaseString : public Utils::CopyableAndMoveable
    {
    public:
        using CharT = CharType;
        using Self = BaseString<CharT>;
        using Toolset = _StringToolset<CharT>;
        using Settings = _StringSettings<CharT>;
        using SizeT = typename Settings::SizeT;
        using HashT = typename Settings::HashT;
        using IndexT = typename Settings::IndexT;
        using StdStringT = typename Toolset::StdStringT;
        using StdStringViewT = typename Toolset::StdStringViewT;
        using StringDataReadOnlyT = StringDataReadOnly<CharT>;
        using StringPool = _StringPool<CharT>;
        using StdRegex = typename Toolset::StdRegex;

        using value_type = CharT;
        using pointer = value_type*;
        using difference_type = long long;

    public:
        template<bool IsReversed>
        class Iterator : public IRandomAccessIterator<CharT, Iterator<IsReversed>, Utils::CopyableAndMoveable, true>
        {
        public:
            using Self = Iterator;
            using Super = IRandomAccessIterator<CharT, Iterator, Utils::CopyableAndMoveable, true>;
            using iterator_category = std::random_access_iterator_tag;
            using value_type = typename BaseString<CharT>::value_type;
            using difference_type = typename BaseString<CharT>::difference_type;
            using pointer = typename BaseString<CharT>::pointer;
            using reference = value_type&;

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

            Self operator+(int step) const noexcept override { return Self{ _data + (IsReversed ? -step : step), _owner }; }

            Self operator-(int step) const noexcept override { return Self{ _data - (IsReversed ? -step : step), _owner }; }

            difference_type operator-(const Self& other) const noexcept
            {
                if (Verify(_owner == other._owner))
                {
                    return _data - other._data;
                }

                return {};
            }

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
            explicit Iterator(CharT* data, const BaseString<CharType>* owner)
                : _data{ data },
                  _owner{ owner }
            {
            }

            [[nodiscard]] Comparison operator<=>(const Self& other) const noexcept
            {
                if (!_data || !other._data)
                {
                    Assert("Impossible to compare two iterators. Some iterator is invalid");
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

                Assert("Impossible to compare two iterators. Was get some error");
                return Comparison::None;
            }

        private:
            CharT* _data = nullptr;
            const BaseString<CharType>* _owner = nullptr;

            friend class BaseString<CharType>;
        };

        using IteratorT = Iterator<false>;
        using ReverseIteratorT = Iterator<true>;
        using StdRegexMatchResults = std::match_results<IteratorT>;

    public:
        [[nodiscard]] IteratorT begin() noexcept { return IteratorT{ _string, this }; }
        [[nodiscard]] const IteratorT begin() const noexcept { return IteratorT{ _string, this }; }
        [[nodiscard]] const IteratorT cbegin() const noexcept { return IteratorT{ _string, this }; }
        [[nodiscard]] IteratorT end() noexcept { return IteratorT{ _string + _size, this }; }
        [[nodiscard]] const IteratorT end() const noexcept { return IteratorT{ _string + _size, this }; }
        [[nodiscard]] const IteratorT cend() const noexcept { return IteratorT{ _string + _size, this }; }

        [[nodiscard]] ReverseIteratorT rbegin() noexcept { return ReverseIteratorT{ _string + _size, this }; }
        [[nodiscard]] const ReverseIteratorT rbegin() const noexcept { return ReverseIteratorT{ _string + _size, this }; }
        [[nodiscard]] const ReverseIteratorT crbegin() const noexcept { return ReverseIteratorT{ _string + _size, this }; }
        [[nodiscard]] ReverseIteratorT rend() noexcept { return ReverseIteratorT{ _string, this }; }
        [[nodiscard]] const ReverseIteratorT rend() const noexcept { return ReverseIteratorT{ _string, this }; }
        [[nodiscard]] const ReverseIteratorT crend() const noexcept { return ReverseIteratorT{ _string, this }; }

        [[nodiscard]] static Self Intern(const CharT* newString) { return Self{ StringPool::Instance().Add(newString, Toolset::Length(newString)) }; }

        [[nodiscard]] static Self Intern(const CharT* newString, SizeT size, bool isCompileTime = false)
        {
            return Self{ StringPool::Instance().Add(newString, size, isCompileTime) };
        }

        [[nodiscard]] static Self Intern(StdStringViewT string) { return Self{ StringPool::Instance().Add(string.data(), string.size()) }; }

        [[nodiscard]] SizeT Size() const noexcept { return _size; }
        [[nodiscard]] SizeT Length() const noexcept { return _size; }
        [[nodiscard]] bool IsEmpty() const noexcept { return _string == nullptr || _size == 0; }
        [[nodiscard]] explicit operator const CharT*() const noexcept { return _string; }
        [[nodiscard]] operator StdStringViewT() const noexcept { return ToStringView(); }
        [[nodiscard]] CharT operator[](IndexT index) const noexcept { return _string[index]; }

        [[nodiscard]] bool operator==(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                if (IsEmpty() && other.IsEmpty())
                {
                    return true;
                }
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return IsStatic() && other.IsStatic() ? _string == other._string : Toolset::Cmp(_string, other._string) == Comparison::Equal;
        }

        [[nodiscard]] bool operator!=(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                if ((IsEmpty() && !other.IsEmpty()) || (!IsEmpty() && other.IsEmpty()))
                {
                    return true;
                }

                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return IsStatic() && other.IsStatic() ? _string != other._string : Toolset::Cmp(_string, other._string) != Comparison::Equal;
        }

        [[nodiscard]] bool operator>(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other._string) == Comparison::Greater;
        }

        [[nodiscard]] bool operator>=(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Greater || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator<(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other._string) == Comparison::Less;
        }

        [[nodiscard]] bool operator<=(const Self& other) const
        {
            if (IsEmpty() || other.IsEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                if (_string && _string[0] == 0 && other && other[0] == 0)
                {
                    return true;
                }

                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Equal;
        }

        [[nodiscard]] bool operator!=(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) != Comparison::Equal;
        }

        [[nodiscard]] bool operator>(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Greater;
        }

        [[nodiscard]] bool operator>=(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Greater || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator<(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Less;
        }

        [[nodiscard]] bool operator<=(const CharT* other) const
        {
            if (IsEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(StdStringViewT other) const
        {
            if (IsEmpty() || other.empty())
            {
                if (_string && _string[0] == 0 && other.empty())
                {
                    return true;
                }
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this == other.data();
        }

        [[nodiscard]] bool operator>(StdStringViewT other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this > other.data();
        }

        [[nodiscard]] bool operator>=(StdStringViewT other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this >= other.data();
        }

        [[nodiscard]] bool operator<(StdStringViewT other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this < other.data();
        }

        [[nodiscard]] bool operator<=(StdStringViewT other) const
        {
            if (IsEmpty() || other.empty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this <= other.data();
        }

        [[nodiscard]] bool operator!() const noexcept { return IsEmpty(); }
        [[nodiscard]] explicit operator bool() const noexcept { return !IsEmpty(); }

        [[nodiscard]] CharT Front() const
        {
            if (IsEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }

            return _string[0];
        }

        [[nodiscard]] CharT Back() const
        {
            if (IsEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }

            return _string[_size - static_cast<SizeT>(1)];
        }

        [[nodiscard]] StdStringViewT ToStringView() const
        {
            if (IsEmpty())
            {
                if constexpr (sizeof(CharT) == 1)
                {
                    return { "" };
                }
                else
                {
                    return { L"" };
                }
            }

            return { _string, _size };
        }

        [[nodiscard]] StdStringT ToStdString() const
        {
            if (IsEmpty())
            {
                return {};
            }

            return { _string };
        }

        [[nodiscard]] CharT At(IndexT index) const noexcept
        {
            if (!Verify(!IsEmpty() && _size < index, "Impossible to work with nullptr string. or invalid index."))
            {
                return {};
            }

            return _string[index];
        }

        [[nodiscard]] const CharT* c_str() const noexcept { return _string; }
        [[nodiscard]] const CharT* CStr() const noexcept { return c_str(); }

        [[nodiscard]] const CharT* data() const noexcept { return _string; }
        [[nodiscard]] const CharT* Data() const noexcept { return data(); }

        [[nodiscard]] CharT* data() noexcept
        {
            TryToMakeAsDynamic();
            return _string;
        }

        [[nodiscard]] CharT* Data() noexcept { return data(); }

        [[nodiscard]] std::vector<Self> Split(const Self& delimiter) const
        {
            if (!Verify(!IsEmpty(), "Impossible to work with nullptr string."))
            {
                return {};
            }

            Self string = _string;

            std::vector<Self> splittedStrings;
            CharT* context = nullptr;

            if (const auto* token = Toolset::StrTok(string.data(), delimiter.c_str(), context))
            {
                splittedStrings.emplace_back(token);
            }
            else
            {
                return {};
            }

            do
            {
                if (const auto* token = Toolset::StrTok(context, delimiter.c_str(), context))
                {
                    splittedStrings.emplace_back(Self{ token });
                }
                else
                {
                    break;
                }
            } while (context);

            return splittedStrings;
        }

        template<class T>
        static Self MakeFrom(T);

        template<>
        static Self MakeFrom(int value)
        {
            Self temp;
            temp.Reserve(16);
            Toolset::FromInt(value, temp.Data(), temp.Capacity());
            temp._size = Toolset::Length(temp.Data());
            return temp;
        }

        template<>
        static Self MakeFrom(const Self& value)
        {
            return value;
        }

        template<>
        static Self MakeFrom(const CharT* value)
        {
            return Self(value);
        }

        template<>
        static Self MakeFrom(float value)
        {
            Self temp;
            temp.Reserve(16);
            Toolset::FromFloat(value, temp.Data(), temp.Capacity());
            temp._size = Toolset::Length(temp.Data());
            return temp;
        }

        template<>
        static Self MakeFrom(double value)
        {
            Self temp;
            temp.Reserve(16);
            Toolset::FromDouble(value, temp.Data(), temp.Capacity());
            temp._size = Toolset::Length(temp.Data());
            return temp;
        }

        template<>
        static Self MakeFrom(unsigned long long value)
        {
            Self temp;
            temp.Reserve(32);
            Toolset::FromUnsignedLongLong(value, temp.Data(), temp.Capacity());
            temp._size = Toolset::Length(temp.Data());
            return temp;
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
            Assert("Impossible to work with nullptr string.");
            return {};
        }

        template<>
        [[nodiscard]] float ConvertTo() const noexcept
        {
            if (!IsEmpty())
            {
                return Toolset::ToFloat(_string);
            }
            Assert("Impossible to work with nullptr string.");
            return {};
        }

        template<>
        [[nodiscard]] double ConvertTo() const noexcept
        {
            if (!IsEmpty())
            {
                return Toolset::ToDouble(_string);
            }
            Assert("Impossible to work with nullptr string.");
            return {};
        }

        template<>
        [[nodiscard]] long long ConvertTo() const noexcept
        {
            if (!IsEmpty())
            {
                return Toolset::ToLongLong(_string);
            }
            Assert("Impossible to work with nullptr string.");
            return {};
        }

        template<IsFormattableType... T>
        [[nodiscard]] static Self Format(StdStringViewT str, const T... args)
        {
            Self temp(str);
            const void* expr = nullptr;
            if constexpr (sizeof(CharT) == 1)
            {
                expr = "\\{\\}";
            }
            else
            {
                expr = L"\\{\\}";
            }

            (temp.RegexReplace(static_cast<const CharT*>(expr), MakeFrom(args), std::regex_constants::match_flag_type::format_first_only), ...);
            return temp;
        }

        [[nodiscard]] HashT MakeHash() const noexcept
        {
            if (!IsEmpty())
            {
                return std::hash<StdStringViewT>{}({ _string, _size });
            }
            Assert("Impossible to make a hash from nullptr string.");
            return {};
        }

        Self& SubStr(IndexT index, SizeT count = 0) noexcept
        {
            if (!IsEmpty())
            {
                const SizeT finalCount = count == 0 ? _size - index : count - index;
                *this = std::move(Self(_string + index, finalCount));
            }

            return *this;
        }

        Self& TrimStart(CharT ch) noexcept
        {
            if (!IsEmpty())
            {
                SizeT offset = 0;
                while (_string[offset] == ch && offset < _size)
                {
                    ++offset;
                }
                if (offset != 0)
                {
                    *this = std::move(Self(_string + offset, _size - offset));
                }
            }

            return *this;
        }

        Self& TrimEnd(CharT ch) noexcept
        {
            if (!IsEmpty())
            {
                SizeT count = 0;
                const CharT* end = _string + _size;
                while (*--end == ch && count < _size)
                {
                    ++count;
                }

                if (count != 0)
                {
                    Resize(_size - count);
                }
            }

            return *this;
        }

        Self& Trim(CharT ch) noexcept { return TrimStart(ch).TrimEnd(ch); }

        Self& ToUpperCase() noexcept
        {
            if (!IsEmpty())
            {
                TryToMakeAsDynamic();
                for (IndexT i = 0; i < _size; ++i)
                {
                    _string[i] = Toolset::ToUpper(_string[i]);
                }
            }

            return *this;
        }

        Self& ToLowerCase() noexcept
        {
            if (!IsEmpty())
            {
                TryToMakeAsDynamic();
                for (IndexT i = 0; i < _size; ++i)
                {
                    _string[i] = Toolset::ToLower(_string[i]);
                }
            }

            return *this;
        }

        Self& Erase(IndexT index)
        {
            if (!IsEmpty())
            {
                if (!Verify(index < _size, "Invalid index"))
                {
                    return *this;
                }

                Self temp(_string, index);
                temp += _string + index + 1;
                *this = std::move(temp);
            }

            return *this;
        }

        Self& Erase(IndexT from, IndexT to)
        {
            if (!IsEmpty())
            {
                if (!Verify(from < _size && to < _size, "Invalid index"))
                {
                    return *this;
                }

                Self temp(_string, from);
                temp += _string + to + 1;
                *this = std::move(temp);
            }

            return *this;
        }

        Self& Erase(IteratorT iterator)
        {
            if (!IsEmpty())
            {
                if (Verify(iterator._owner == this && iterator._data, "Was passed an invalid iterator"))
                {
                    return Erase(iterator._data - _string);
                }
            }
            return *this;
        }

        Self& Erase(IteratorT from, IteratorT to)
        {
            if (!IsEmpty())
            {
                if (Verify(from._owner == this && from._data, "Was passed an invalid iterator 'from'") &&
                    Verify(to._owner == this && to._data, "Was passed an invalid iterator 'to'"))
                {
                    return Erase(from._data - _string, to._data - _string);
                }
            }
            return *this;
        }

        Self& ReplaceFirst(StdStringViewT mainValue, StdStringViewT newValue) noexcept
        {
            if (!IsEmpty())
            {
                if (auto* found = Find(mainValue))
                {
                    auto temp = Self(_string, found - _string);
                    temp += newValue;
                    temp += found + mainValue.size();
                    *this = std::move(temp);
                }
            }

            return *this;
        }

        Self& ReplaceAll(StdStringViewT mainValue, StdStringViewT newValue) noexcept
        {
            if (!IsEmpty())
            {
                int offset = 0;
                // TODO: optimize this code
                while (auto* found = Find(mainValue, offset))
                {
                    offset = found - _string + newValue.size();
                    auto temp = Self(_string, found - _string);
                    temp += newValue;
                    temp += found + mainValue.size();
                    *this = std::move(temp);
                }
            }

            return *this;
        }

        [[nodiscard]] static bool IsSpace(CharT ch) noexcept { return Toolset::IsSpace(ch); }
        [[nodiscard]] static bool IsContainChar(CharT ch, StdStringViewT set) noexcept
        {
            for (const auto value : set)
            {
                if (value == ch)
                {
                    return true;
                }
            }

            return false;
        }

        [[nodiscard]] bool RegexMatch(StdStringViewT expr, std::regex_constants::match_flag_type flag = std::regex_constants::match_default) const
        {
            if (!IsEmpty())
            {
                return std::regex_match(_string, StdRegex(expr.data()), flag);
            }

            return false;
        }

        [[nodiscard]] bool RegexMatch(StdStringViewT expr, std::match_results<Iterator<false>>& match,
                                      std::regex_constants::match_flag_type flag = std::regex_constants::match_default) const
        {
            if (!IsEmpty())
            {
                return std::regex_match(begin(), end(), match, StdRegex(expr.data()), flag);
            }

            return false;
        }

        bool RegexReplace(StdStringViewT expr, StdStringViewT newValue,
                          std::regex_constants::match_flag_type flag = std::regex_constants::match_default)
        {
            const StdRegex regex(expr.data());
            BaseString temp;
            std::regex_replace(std::back_inserter(temp), begin(), end(), regex, newValue.data(), flag);
            const auto wasReplaced = *this != temp;
            *this = std::move(temp);
            return wasReplaced;
        }

        [[nodiscard]] Self GetCopyAsDynamic() const { return BaseString(_string, _size); }

        Self& operator+=(CharT ch) noexcept { return push_back(ch); }
        Self& operator+=(StdStringViewT str) noexcept { return push_back(str); }

        Self& push_back(CharT ch) noexcept { return push_back(StdStringViewT(&ch, 1)); }
        Self& PushBack(CharT ch) noexcept { return push_back(StdStringViewT(&ch, 1)); }

        Self& push_back(StdStringViewT str) noexcept { return PushBack(str); }

        Self& PushBack(StdStringViewT str) noexcept
        {
            const auto oldSize = _size;
            const auto finalSize = _size + str.size();
            if (finalSize >= _capacity)
            {
                Reserve(finalSize);
            }

            for (IndexT i = oldSize, j = 0; i < finalSize && j < str.size(); ++i, ++j)
            {
                _string[i] = str[j];
            }
            _string[finalSize] = 0;
            _size += str.size();

            return *this;
        }

        Self& push_front(CharT ch) noexcept { return PushFront(StdStringViewT(&ch, 1)); }
        Self& PushFront(CharT ch) noexcept { return PushFront(StdStringViewT(&ch, 1)); }

        Self& push_front(StdStringViewT str) noexcept { return PushFront(str); }

        Self& PushFront(StdStringViewT str) noexcept
        {
            const auto oldSize = _size;
            const auto finalSize = _size + str.size();
            if (finalSize >= _capacity)
            {
                Reserve(finalSize);
            }

            for (long long i = oldSize; i >= 0; --i)
            {
                _string[i + str.size()] = _string[i];
            }

            _size += str.size();
            for (IndexT i = 0; i < str.size() && i < _size; ++i)
            {
                _string[i] = str[i];
            }

            return *this;
        }

        Self& PopBack() noexcept { return pop_back(); }

        Self& pop_back() noexcept
        {
            if (Verify(_size > 0, "Impossible to pop_back a value from the empty string"))
            {
                TryToMakeAsDynamic();
                _string[--_size] = 0;
            }
            return *this;
        }

        Self& PopFront() noexcept { return pop_front(); }

        Self& pop_front() noexcept
        {
            if (Verify(_size > 0, "Impossible to pop_back a value from the empty string"))
            {
                TryToMakeAsDynamic();
                for (long long i = 1ll; i < _size; ++i)
                {
                    _string[i - 1ll] = _string[i];
                }
                _string[--_size] = 0;
            }
            return *this;
        }

        const Self& Copy(CharT* dest, SizeT count, SizeT offset = 0) const
        {
            if (!IsEmpty())
            {
                memcpy_s(dest, count * sizeof(CharT), _string + offset, std::min(_size - offset, count) * sizeof(CharT));
                dest[count] = 0;
            }
            return *this;
        }

        Self& ShrinkToFit() noexcept { return shrink_to_fit(); }

        Self& shrink_to_fit() noexcept
        {
            const auto* oldString = _string;
            const auto capacity = _size + static_cast<SizeT>(1);

            if ((_string = new CharT[capacity]))
            {
                _capacity = capacity;
                memcpy_s(_string, _size * sizeof(CharT), oldString, _size);
                _string[_size] = 0;

                if (_policy == StringPolicy::Static)
                {
                    _policy = StringPolicy::Dynamic;
                }
                else if (_policy == StringPolicy::Dynamic)
                {
                    delete[] oldString;
                }
            }

            return *this;
        }

        [[nodiscard]] SizeT Capacity() const noexcept { return _capacity; }

        Self& Insert(IteratorT iterator, const CharT* str, SizeT size = Settings::invalidSize) noexcept
        {
            return insert(std::move(iterator), str, size);
        }

        Self& insert(IteratorT iterator, const CharT* str, SizeT size = Settings::invalidSize) noexcept
        {
            Assert(iterator._owner == this);
            if (iterator._owner == this)
            {
                return insert(iterator._data - _string, str, size);
            }

            return *this;
        }

        Self& Insert(long long pos, const CharT* str, SizeT size = Settings::invalidSize) noexcept { return insert(pos, str, size); }

        Self& insert(long long pos, const CharT* str, SizeT size = Settings::invalidSize) noexcept
        {
            if (size == Settings::invalidSize)
            {
                size = Toolset::Length(str);
            }

            const auto oldSize = _size;
            const auto finalSize = _size + size;
            if (finalSize >= _capacity)
            {
                Reserve(finalSize);
            }

            for (long long i = oldSize; i >= pos; --i)
            {
                _string[i + size] = _string[i];
            }

            for (IndexT i = pos; i < pos + size; ++i, ++str)
            {
                _string[i] = *str;
            }
            _string[finalSize] = 0;
            _size += size;

            return *this;
        }

        [[nodiscard]] bool IsStatic() const noexcept { return _policy == StringPolicy::Static; }
        [[nodiscard]] bool IsDynamic() const noexcept { return _policy == StringPolicy::Dynamic; }
        [[nodiscard]] bool CheckForPolicy(StringPolicy policy) const noexcept { return _policy == policy; }

        [[nodiscard]] Comparison Compare(StdStringViewT other, const bool isIgnoreCase = false) const noexcept
        {
            if (!Verify(!IsEmpty() && !other.empty(), "Impossible to work with nullptr string."))
            {
                return Comparison::None;
            }

            if (isIgnoreCase)
            {
                for (IndexT index = 0; _string[index] && index < other.size(); ++index)
                {
                    if (_string[index + 1ull] == 0 && other.size() == index + 1ull)
                    {
                        return Comparison::Equal;
                    }

                    const auto diff = Toolset::ToUpper(_string[index]) - Toolset::ToUpper(other[index]);
                    if (diff > 0 || _string[index + 1ull] == 0)
                    {
                        return Comparison::Greater;
                    }
                    if (diff < 0 || other.size() == index + 1ull)
                    {
                        return Comparison::Less;
                    }
                }
                return Comparison::None;
            }

            return Toolset::Cmp(_string, other.data());
        }

        [[nodiscard]] StdRegexMatchResults FindRegex(StdStringViewT expr, int baseOffset = 0,
                                                     std::regex_constants::match_flag_type flag = std::regex_constants::match_default) const
        {
            if (!Verify(!IsEmpty() && !expr.empty(), "Impossible to work with nullptr string."))
            {
                return {};
            }

            StdRegexMatchResults match;
            std::regex_search(begin() + baseOffset, end(), match, StdRegex(expr.data()), flag);

            return match;
        }

        void IterateRegex(StdStringViewT expr, std::function<bool(const StdRegexMatchResults&)>&& lambda, int baseOffset = 0,
                          std::regex_constants::match_flag_type flag = std::regex_constants::match_default) const
        {
            if (!Verify(!IsEmpty() && !expr.empty(), "Impossible to work with nullptr string."))
            {
                return;
            }

            StdRegex regexExpr(expr.data());
            auto first = std::regex_iterator<IteratorT>(begin() + baseOffset, end(), regexExpr, flag);
            auto last = std::regex_iterator<IteratorT>();
            for (; first != last; ++first)
            {
                if (lambda)
                {
                    if (!std::invoke(lambda, *first))
                    {
                        break;
                    }
                }
            }
        }

        [[nodiscard]] const CharT* Find(StdStringViewT other, int baseOffset = 0) const noexcept
        {
            if (!Verify(!IsEmpty() && !other.empty(), "Impossible to work with nullptr string."))
            {
                return nullptr;
            }

            return Toolset::StrStr(_string + baseOffset, other.data());
        }

        [[nodiscard]] std::vector<const CharT*> FindAll(StdStringViewT other) const noexcept
        {
            if (!Verify(!IsEmpty() && !other.empty(), "Impossible to work with nullptr string."))
            {
                return {};
            }

            std::vector<const CharT*> strings;

            CharT* foundStr = _string;
            do
            {
                if ((foundStr = Toolset::StrStr(foundStr, other.data())))
                {
                    strings.push_back(foundStr);
                    ++foundStr;
                }
            } while (foundStr);

            return strings;
        }

        BaseString() = default;

        template<class IterT>
        BaseString(IterT first, IterT last)
        {
            if (first != last)
            {
                constexpr const SizeT defaultCapacity = 16;
                Reserve(defaultCapacity);
                for (; first != last; ++first)
                {
                    PushBack(static_cast<CharT>(0));
                    std::char_traits<CharT>::assign(_string[_size - static_cast<SizeT>(1)], *first);
                }
            }
        }

        BaseString(const CharT* str, SizeT size = Settings::invalidSize)
        {
            Resize(size == Settings::invalidSize ? Toolset::Length(str) : size);
            memcpy_s(_string, _size * sizeof(CharT), str, _size * sizeof(CharT));
        }

        explicit BaseString(StdStringViewT str)
            : BaseString(str.data(), str.size())
        {
        }

        BaseString(const Self& other) { *this = other; }

        explicit BaseString(SizeT reserveCount) { Reserve(reserveCount); }

        Self& operator=(StdStringViewT other)
        {
            Clear();
            Resize(other.size());
            memcpy_s(_string, _size * sizeof(CharT), other.data(), other.size() * sizeof(CharT));
            return *this;
        }

        Self& operator=(const Self& other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (other._policy == StringPolicy::Dynamic)
            {
                Clear();
                Resize(other._size);
                memcpy_s(_string, _size * sizeof(CharT), other._string, other._size * sizeof(CharT));
            }
            else if (other._policy == StringPolicy::Static)
            {
                Clear();
                _policy = other._policy;
                _string = other._string;
                _size = other._size;
                _capacity = other._capacity;
            }
            else
            {
                Clear();
            }

            return *this;
        }

        BaseString(Self&& other) noexcept { *this = std::move(other); }

        Self& operator=(Self&& other) noexcept
        {
            if (other._policy == StringPolicy::Dynamic)
            {
                Clear();
                _string = other._string;
                _size = other._size;
                _policy = StringPolicy::Dynamic;
                _capacity = other._capacity;

                other._size = 0;
                other._string = nullptr;
                other._policy = StringPolicy::None;
                other._capacity = 0;
            }
            else if (other._policy == StringPolicy::Static)
            {
                Clear();
                _policy = StringPolicy::Static;
                _string = other._string;
                _size = other._size;
                _capacity = other._capacity;

                other._size = 0;
                other._string = nullptr;
                other._policy = StringPolicy::None;
                other._capacity = 0;
            }
            else
            {
                Clear();
            }

            return *this;
        }

        void Clear()
        {
            if (_string)
            {
                if (_policy == StringPolicy::Static)
                {
                    _string = nullptr;
                    _size = 0;
                    _policy = StringPolicy::None;
                    _capacity = 0;
                }
                else if (_policy == StringPolicy::Dynamic)
                {
                    delete[] _string;
                    _string = nullptr;
                    _size = 0;
                    _policy = StringPolicy::None;
                    _capacity = 0;
                }
                else
                {
                    Assert("Invalid StringPolicy type. Impossible to delete.");
                }
            }
        }

        Self& Reserve(const SizeT newSize)
        {
            const auto* oldString = _string;
            const auto oldCapacity = _capacity;

            const SizeT finalCapacity = newSize * _capacityMultiplier + static_cast<SizeT>(1);
            if (auto* newString = new CharT[finalCapacity]{})
            {
                if (_string)
                {
                    const auto limit = std::min(finalCapacity, oldCapacity);
                    for (IndexT i = 0; i < limit; ++i)
                    {
                        newString[i] = _string[i];
                    }
                }

                if (_policy == StringPolicy::Static)
                {
                    _string = nullptr;
                }
                else if (_policy == StringPolicy::Dynamic)
                {
                    delete[] _string;
                }
                _string = newString;
                _capacity = finalCapacity;
                _policy = StringPolicy::Dynamic;
                if (newSize < oldCapacity)
                {
                    _size = newSize;
                    _string[_size] = 0;
                }
            }

            return *this;
        }

        Self& Resize(const SizeT newSize)
        {
            if (newSize < _size && _policy != StringPolicy::Static)
            {
                _string[newSize] = 0;
            }
            else if (newSize > _size || _policy == StringPolicy::Static)
            {
                Reserve(newSize);
            }
            // for empty init
            else if (newSize == 0 && _policy == StringPolicy::None)
            {
                Reserve(1);
            }
            _size = newSize;

            return *this;
        }

        [[nodiscard]] BaseString<char> ToASCII() const
        {
            BaseString<char> temp;
            for (auto ch : *this)
            {
                temp += static_cast<char>(ch);
            }
            return temp;
        }

        ~BaseString() override { Clear(); }

        // ============= Utils ===============
        static std::size_t GetLinesCountInText(const Self& source, const CharT* end) noexcept
        {
            if (!Verify(!end || !source.IsEmpty(), "Impossible to calculate count of lines in thext, because was passed NULL pointer to the string."))
            {
                return 0;
            }
            constexpr const int endLineCode = 10; // 10 == '\n'

            while (end[0] == endLineCode)
            {
                ++end;
            }

            std::size_t count = 0;
            for (std::size_t i = 0; i < end - source.c_str(); ++i)
            {
                if (source[i] == endLineCode)
                {
                    ++count;
                }
            }

            return ++count;
        }

    protected:
        explicit BaseString(StringDataReadOnlyT data)
            : _string{ data.str },
              _size{ data.size },
              _capacity{ data.size + static_cast<SizeT>(1) },
              _policy{ StringPolicy::Static }
        {
        }

        void TryToMakeAsDynamic()
        {
            if (_policy != StringPolicy::Dynamic && !IsEmpty())
            {
                Reserve(_size);
            }
        }

    protected:
        CharT* _string = nullptr;
        SizeT _size = 0;
        SizeT _capacity = 0;
        StringPolicy _policy = StringPolicy::None;
        static constexpr SizeT _capacityMultiplier = 2ull;
    };

    using StringAtom = BaseString<char>;
    using WStringAtom = BaseString<wchar_t>;
} // namespace Core

template<class CharType>
struct std::hash<Core::BaseString<CharType>>
{
    size_t operator()(const Core::BaseString<CharType>& x) const noexcept { return x.MakeHash(); }
};

inline Core::BaseString<char> operator""_atom(const char* str, std::size_t size) noexcept
{
    return Core::BaseString<char>::Intern(str, size, true);
}

inline Core::BaseString<wchar_t> operator""_atom(const wchar_t* str, std::size_t size) noexcept
{
    return Core::BaseString<wchar_t>::Intern(str, size, true);
}

template<class CharType>
[[nodiscard]] bool operator>(typename Core::_StringToolset<CharType>::StdStringViewT str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1);
}

template<class CharType>
[[nodiscard]] bool operator>=(typename Core::_StringToolset<CharType>::StdStringViewT str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1) || (str1 == str2);
}

template<class CharType>
[[nodiscard]] bool operator<(typename Core::_StringToolset<CharType>::StdStringViewT str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1);
}

template<class CharType>
[[nodiscard]] bool operator<=(typename Core::_StringToolset<CharType>::StdStringViewT str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1) || (str1 == str2);
}