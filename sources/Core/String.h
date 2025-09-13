//  MIT License
//
//  Copyright (c) 2019-2025 Valerii Koniushenko
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include "Core/AbstractIterators.h"
#include "Core/Assert.h"
#include "Core/CommonEnums.h"
#include "Regex.h"
#include "Singleton.h"
#include "Utils/CopyableAndMoveableBehaviour.h"
#include "Utils/CrossString.h"
#include "Utils/TypeTraits.h"

#include <algorithm>
#include <cinttypes>
#include <codecvt>
#include <cstring>
#include <cwctype>
#include <filesystem>
#include <functional>
#include <optional>
#include <regex>
#include <type_traits>
#include <unordered_map>
#include <vector>

#ifdef UTILS_DEBUG
    #include <sstream>
#endif

namespace Core
{
    enum class StringPolicy : uint8_t
    {
        None,
        Static,
        Dynamic
    };

    template<class CharType>
    struct _StringSettings : public Utils::Abstract
    {
        using CharT = CharType;
        using SizeT = uint64_t;
        using HashT = uint64_t;
        using IndexT = uint64_t;
        constexpr static SizeT invalidSize = ~static_cast<SizeT>(0);
    };

    template<class CharType>
    struct _StringToolset;

    template<>
    struct _StringToolset<char> : public Utils::Abstract
    {
        using CharT = char;
        using StdStringT [[maybe_unused]] = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StdStringViewT = std::basic_string_view<CharT, std::char_traits<CharT>>;
        using SizeT = typename _StringSettings<CharT>::SizeT;

        [[nodiscard]] static bool IsSpace(int ch) { return static_cast<bool>(isspace(ch)); }
        [[nodiscard]] static SizeT Length(const CharT* string) noexcept { return static_cast<SizeT>(strlen(string)); }

        [[nodiscard, maybe_unused]] static int ToInt(const CharT* str) noexcept { return atoi(str); }
        [[nodiscard, maybe_unused]] static float ToFloat(const CharT* str) noexcept { return static_cast<float>(atof(str)); }
        [[nodiscard, maybe_unused]] static double ToDouble(const CharT* str) noexcept { return atof(str); }
        [[nodiscard, maybe_unused]] static int64_t ToInt64(const CharT* str) noexcept { return atoll(str); }
        [[nodiscard, maybe_unused]] static uint64_t ToUInt64(const CharT* str) noexcept { return atoll(str); }

        [[maybe_unused]] static void FromInt32(int32_t value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%d", value); errorCode < 0)
            {
                Assert("Impossible to convert 'int32_t' value to string.");
            }
        }

        [[maybe_unused]] static void FromFloat(float value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%f", value); errorCode < 0)
            {
                Assert("Impossible to convert 'float' value to string.");
            }
        }

        [[maybe_unused]] static void FromDouble(double value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%lf", value); errorCode < 0)
            {
                Assert("Impossible to convert 'double' value to string.");
            }
        }

        [[maybe_unused]] static void FromStdFilesystemPath(const std::filesystem::path& value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%s", value.generic_string().c_str()); errorCode < 0)
            {
                Assert("Impossible to convert 'std::filesystem::path' value to string.");
            }
        }

        [[maybe_unused]] static void FromUInt64(uint64_t value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%" PRIu64, value); errorCode < 0)
            {
                Assert("Impossible to convert 'uint64_t' value to string.");
            }
        }

        [[maybe_unused]] static void FromInt64(int64_t value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = snprintf(buffer, bufferSize, "%" PRId64, value); errorCode < 0)
            {
                Assert("Impossible to convert 'int64_t' value to string.");
            }
        }

        [[nodiscard, maybe_unused]] static CharT* StrTok(CharT* string, const CharT* delim, CharT*& context) noexcept
        {
            return strtok_s(string, delim, &context);
        };
        [[nodiscard, maybe_unused]] static CharT* StrStr(CharT* mainString, const CharT* subString) noexcept
        {
            return strstr(mainString, subString);
        };
        [[nodiscard, maybe_unused]] static const CharT* StrStr(const CharT* mainString, const CharT* subString) noexcept
        {
            return strstr(mainString, subString);
        };

        [[nodiscard, maybe_unused]] static int ToUpper(const CharT ch) noexcept { return toupper(ch); };
        [[nodiscard, maybe_unused]] static int ToLower(const CharT ch) noexcept { return tolower(ch); };

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

        static const CharT* ReverseStrStr(const CharT* string, const CharT* substr, const CharT* end = nullptr)
        {
            if (!string)
            {
                return nullptr;
            }

            if (!substr)
            {
                return string;
            }

            if (*substr == 0) // 0 is '\0'
            {
                return string;
            }
            const auto lenStr = Length(string);
            const auto limitOffset = end ? (string + lenStr) - end : 0;
            const auto lenSubstr = Length(substr);
            if (lenSubstr > lenStr)
            {
                return nullptr;
            }

            for (const CharT* p = string + lenStr - lenSubstr - limitOffset; p >= string; --p)
            {
                if (memcmp(p, substr, lenSubstr * sizeof(CharT)) == 0 || p == string)
                {
                    return p;
                }
            }

            return nullptr;
        }

        [[maybe_unused]] static CharT* ReverseStrStr(CharT* string, const CharT* substr, const CharT* end = nullptr)
        {
            return const_cast<CharT*>(ReverseStrStr(static_cast<const CharT*>(string), substr, static_cast<const CharT*>(end)));
        }
    };

    template<>
    struct _StringToolset<wchar_t> : public Utils::Abstract
    {
        using CharT = wchar_t;
        using StdStringT [[maybe_unused]] [[maybe_unused]] = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;
        using StdStringViewT = std::basic_string_view<CharT, std::char_traits<CharT>>;
        using SizeT = typename _StringSettings<CharT>::SizeT;

        [[nodiscard]] static bool IsSpace(wint_t ch) { return static_cast<bool>(std::iswspace(ch)); }
        [[nodiscard]] static SizeT Length(const CharT* string) noexcept { return static_cast<SizeT>(wcslen(string)); }

        [[nodiscard, maybe_unused]] static int ToInt(const CharT* str) noexcept { return _wtoi(str); }
        [[nodiscard, maybe_unused]] static float ToFloat(const CharT* str) noexcept { return static_cast<float>(_wtof(str)); }
        [[nodiscard, maybe_unused]] static double ToDouble(const CharT* str) noexcept { return _wtof(str); }
        [[nodiscard, maybe_unused]] static int64_t ToInt64(const CharT* str) noexcept { return _wtoll(str); }
        [[nodiscard, maybe_unused]] static uint64_t ToUInt64(const CharT* str) noexcept { return _wtoll(str); }

        [[maybe_unused]] static void FromInt32(int32_t value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%d", value); errorCode < 0)
            {
                Assert("Impossible to convert 'int32_t' value to string.");
            }
        }

        [[maybe_unused]] static void FromFloat(float value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%f", value); errorCode < 0)
            {
                Assert("Impossible to convert 'float' value to string.");
            }
        }

        [[maybe_unused]] static void FromDouble(double value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%lf", value); errorCode < 0)
            {
                Assert("Impossible to convert 'double' value to string.");
            }
        }

        [[maybe_unused]] static void FromStdFilesystemPath(const std::filesystem::path& value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%s", value.wstring().c_str()); errorCode < 0)
            {
                Assert("Impossible to convert 'std::filesystem::path' value to string.");
            }
        }

        [[maybe_unused]] static void FromUInt64(uint64_t value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%llu", value); errorCode < 0)
            {
                Assert("Impossible to convert 'uint64_t' value to string.");
            }
        }

        [[maybe_unused]] static void FromInt64(int64_t value, CharT* buffer, SizeT bufferSize)
        {
            if (const auto errorCode = _snwprintf_s(buffer, bufferSize, bufferSize, L"%lld", value); errorCode < 0)
            {
                Assert("Impossible to convert 'int64_t' value to string.");
            }
        }

        [[nodiscard, maybe_unused]] static CharT* StrTok(CharT* string, const CharT* delim, CharT*& context) noexcept
        {
            return wcstok_s(string, delim, &context);
        };
        [[nodiscard, maybe_unused]] static CharT* StrStr(CharT* mainString, const CharT* subString) noexcept
        {
            return wcsstr(mainString, subString);
        };
        [[nodiscard, maybe_unused]] static const CharT* StrStr(const CharT* mainString, const CharT* subString) noexcept
        {
            return wcsstr(mainString, subString);
        };

        [[nodiscard, maybe_unused]] static int ToUpper(const CharT ch) noexcept { return static_cast<int>(towupper(ch)); };
        [[nodiscard, maybe_unused]] static int ToLower(const CharT ch) noexcept { return static_cast<int>(towlower(ch)); };

        [[nodiscard]] static Comparison Cmp(const CharT* str1, const CharT* str2) noexcept
        {
            const auto result = wcscmp(str1, str2);
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

        static const CharT* ReverseStrStr(const CharT* string, const CharT* substr, const CharT* end = nullptr)
        {
            if (!string)
            {
                return nullptr;
            }

            if (!substr)
            {
                return string;
            }

            if (*substr == 0) // 0 is '\0'
            {
                return string;
            }
            const auto lenStr = Length(string);
            const auto limitOffset = end ? (string + lenStr) - end : 0;
            const auto lenSubstr = Length(substr);
            if (lenSubstr > lenStr)
            {
                return nullptr;
            }

            for (const CharT* p = string + lenStr - lenSubstr - limitOffset; p >= string; --p)
            {
                if (memcmp(p, substr, lenSubstr * sizeof(CharT)) == 0 || p == string)
                {
                    return p;
                }
            }

            return nullptr;
        }

        [[maybe_unused]] static CharT* ReverseStrStr(CharT* string, const CharT* substr, const CharT* end = nullptr)
        {
            return const_cast<CharT*>(ReverseStrStr(static_cast<const CharT*>(string), substr, static_cast<const CharT*>(end)));
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

        [[maybe_unused]] StringData(SmartPointer&& ptr, typename Settings::SizeT newSize)
            : str{ std::move(ptr) },
              size{ newSize }
        {
        }

        [[maybe_unused]] StringData(const CharType* newString, typename Settings::SizeT newSize)
            : str{ SmartPointer(new CharType[newSize + static_cast<decltype(newSize)>(1)]) }
        {
            memcpy_s(str.get(), size * sizeof(CharType), newString, newSize * sizeof(CharType));
        }

        [[nodiscard]] StringDataReadOnly<CharType> toReadOnly() noexcept { return StringDataReadOnly<CharType>{ str.get(), size }; }

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
    class _StringPool : public Core::StrictSingleton<_StringPool<CharType>>
    {
        SINGLETONS_FRIEND(_StringPool)
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
        [[nodiscard]] StringDataReadOnlyT add(const CharT* string, typename Settings::SizeT size)
        {
            const auto currentHash = std::hash<StdStringViewT>{}(StdStringViewT{ string, size });
            auto it = _strings.find(currentHash);
            if (it != _strings.end())
            {
                return it->second.toReadOnly();
            }

            auto&& ptr = std::make_unique<CharT[]>(size + static_cast<typename Settings::SizeT>(1));
            memcpy(ptr.get(), string, size * sizeof(CharT));
            auto* addr = ptr.get();
            _strings.emplace(currentHash, StringDataT{ std::move(ptr), size });

            return StringDataReadOnlyT{ addr, size };
        }

    private:
        std::unordered_map<HashT, StringDataT> _strings = {};
    };

    class Iterator;
    template<class CharType>
    class BaseString;

    template<class T>
    concept IsFormattableType =
        std::is_same_v<std::decay_t<T>, int> || std::is_same_v<std::decay_t<T>, double> || std::is_same_v<std::decay_t<T>, float> ||
        std::is_same_v<std::decay_t<T>, uint64_t> || std::is_same_v<std::decay_t<T>, const char*> || std::is_same_v<std::decay_t<T>, char*> ||
        std::is_same_v<std::decay_t<T>, const wchar_t*> || std::is_same_v<std::decay_t<T>, wchar_t*> ||
        std::is_same_v<BaseString<typename T::CharT>, T> || std::is_same_v<std::string_view, T> || std::is_same_v<std::string, T>;

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

        using value_type = CharT;
        using pointer = value_type*;
        using difference_type = int64_t;

        template<bool IsConst>
        using AdaptiveRawPtr = std::conditional_t<IsConst, const Self, Self>*;

        enum LineSeparator : uint8_t
        {
            CR,
            LF,
            CRLF,
            LFCR
        };

    public:
        template<bool IsReversed>
        class BaseIterator : public IRandomAccessIterator<CharT, BaseIterator<IsReversed>, Utils::CopyableAndMoveable, true>
        {
        public:
            using Self = BaseIterator;
            using Super = IRandomAccessIterator<CharT, BaseIterator, Utils::CopyableAndMoveable, true>;
            using iterator_category = std::random_access_iterator_tag;
            using value_type = typename BaseString<CharT>::value_type;
            using difference_type = typename BaseString<CharT>::difference_type;
            using pointer = typename BaseString<CharT>::pointer;
            using reference = value_type&;

        public:
            BaseIterator() = default;

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

            [[nodiscard]] Self operator+(int step) const noexcept override { return Self{ _data + (IsReversed ? -step : step), _owner }; }

            [[nodiscard]] Self operator-(int step) const noexcept override { return Self{ _data - (IsReversed ? -step : step), _owner }; }

            difference_type operator-(const Self& other) const noexcept
            {
                if (_owner == other._owner)
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

            void swap(Self& other) override
            {
                auto temp = *this;
                *this = other;
                other = temp;
            }

        protected:
            explicit BaseIterator(CharT* data, const BaseString<CharType>* owner)
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

        using Iterator = BaseIterator<false>;
        using ConstIterator = const BaseIterator<false>;
        using ReverseIterator = BaseIterator<true>;
        using ConstReverseIterator = const BaseIterator<true>;

    public:
        [[nodiscard]] Iterator begin() noexcept { return Iterator{ _string, this }; }
        [[nodiscard]] ConstIterator begin() const noexcept { return Iterator{ _string, this }; }
        [[nodiscard]] ConstIterator cbegin() const noexcept { return Iterator{ _string, this }; }
        [[nodiscard]] Iterator end() noexcept { return Iterator{ _string + _size, this }; }
        [[nodiscard]] ConstIterator end() const noexcept { return Iterator{ _string + _size, this }; }
        [[nodiscard]] ConstIterator cend() const noexcept { return Iterator{ _string + _size, this }; }

        [[nodiscard]] ReverseIterator rbegin() noexcept { return ReverseIterator{ _string + _size, this }; }
        [[nodiscard]] ConstReverseIterator rbegin() const noexcept { return ReverseIterator{ _string + _size, this }; }
        [[nodiscard]] ConstReverseIterator crbegin() const noexcept { return ReverseIterator{ _string + _size, this }; }
        [[nodiscard]] ReverseIterator rend() noexcept { return ReverseIterator{ _string, this }; }
        [[nodiscard]] ConstReverseIterator rend() const noexcept { return ReverseIterator{ _string, this }; }
        [[nodiscard]] ConstReverseIterator crend() const noexcept { return ReverseIterator{ _string, this }; }

        /**
         * @brief This function will use the provided string as a static string
         */
        [[nodiscard]] static Self Intern(const CharT* newString) { return Self{ StringPool::instance().add(newString, Toolset::Length(newString)) }; }

        /**
         * @brief This function will use the provided string as a static string
         */
        [[nodiscard]] static Self Intern(const CharT* newString, SizeT size) { return Self{ StringPool::instance().add(newString, size) }; }

        /**
         * @brief This function will use the provided string as a static string
         */
        [[nodiscard]] static Self Intern(StdStringViewT string) { return Self{ StringPool::instance().add(string.data(), string.size()) }; }

        [[nodiscard]] SizeT size() const noexcept { return _size; }
        [[nodiscard]] SizeT byteSize() const noexcept { return _size * sizeof(CharT); }
        [[nodiscard]] SizeT length() const noexcept { return _size; }
        [[nodiscard]] bool isEmpty() const noexcept { return _string == nullptr || _size == 0; }
        [[nodiscard]] explicit operator const CharT*() const noexcept { return _string; }
        [[nodiscard]] operator StdStringViewT() const noexcept { return toStdStringView(); }
        [[nodiscard]] CharT& operator[](IndexT index) noexcept { return _string[index]; }
        [[nodiscard]] CharT operator[](IndexT index) const noexcept { return _string[index]; }

        [[nodiscard]] bool operator==(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                return isEmpty() && other.isEmpty();
            }

            if (isStatic() && other.isStatic())
            {
                return _string == other._string;
            }

            if (_size == other._size)
            {
                return memcmp(_string, other._string, _size * sizeof(CharT)) == 0;
            }

            return Toolset::Cmp(_string, other._string) == Comparison::Equal;
        }

        [[nodiscard]] bool operator!=(const Self& other) const { return !this->operator==(other); }

        [[nodiscard]] bool operator>(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other._string) == Comparison::Greater;
        }

        [[nodiscard]] bool operator>=(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Greater || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator<(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other._string) == Comparison::Less;
        }

        [[nodiscard]] bool operator<=(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                if (((_string && _string[0] == 0) || _string == nullptr) && ((other && other[0] == 0) || other == nullptr))
                {
                    return true;
                }

                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Equal;
        }

        [[nodiscard]] bool operator!=(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) != Comparison::Equal;
        }

        [[nodiscard]] bool operator>(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Greater;
        }

        [[nodiscard]] bool operator>=(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Greater || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator<(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Less;
        }

        [[nodiscard]] bool operator<=(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                if (((_string && _string[0] == 0) || _string == nullptr) && other.empty())
                {
                    return true;
                }
                return false;
            }
            return *this == other.data();
        }

        [[nodiscard]] bool operator>(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this > other.data();
        }

        [[nodiscard]] bool operator>=(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this >= other.data();
        }

        [[nodiscard]] bool operator<(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this < other.data();
        }

        [[nodiscard]] bool operator<=(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }
            return *this <= other.data();
        }

        [[nodiscard]] bool operator!() const noexcept { return isEmpty(); }
        [[nodiscard]] explicit operator bool() const noexcept { return !isEmpty(); }

        [[nodiscard]] CharT front() const
        {
            if (isEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }

            return _string[0];
        }

        [[nodiscard]] CharT back() const
        {
            if (isEmpty())
            {
                Assert("Impossible to work with nullptr string.");
                return {};
            }

            return _string[_size - static_cast<SizeT>(1)];
        }

        [[nodiscard]] StdStringViewT toStdStringView() const
        {
            if (isEmpty())
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

        [[nodiscard]] StdStringT toStdString() const
        {
            if (isEmpty())
            {
                return {};
            }

            return { _string };
        }

        [[nodiscard]] CharT at(IndexT index) const noexcept
        {
            if (!Verify(!isEmpty() && _size < index, "Impossible to work with nullptr string. or invalid index."))
            {
                return {};
            }

            return _string[index];
        }

        [[nodiscard]] const CharT* c_str() const noexcept { return _string; }

        [[nodiscard]] const CharT* data() const noexcept { return _string; }

        [[nodiscard]] CharT* data() noexcept
        {
            tryToMakeAsDynamic();
            return _string;
        }

        [[nodiscard]] std::vector<Self> split(const Self& delimiter) const
        {
            if (isEmpty())
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

        template<class T, class = typename std::enable_if_t<std::is_integral_v<T>>>
        static Self MakeFrom(T value)
        {
            // TODO: optimize it using static array
            Self temp;
            temp.reserve(32);

            if constexpr (std::is_unsigned_v<T>)
            {
                Toolset::FromUInt64(value, temp.data(), temp.capacity());
            }
            else
            {
                Toolset::FromInt64(value, temp.data(), temp.capacity());
            }

            temp._size = Toolset::Length(temp.data());
            temp.shrink_to_fit();

            return temp;
        }

        static Self MakeFrom(const Self& value) { return Self{ value }; }

        static Self MakeFrom(StdStringT value) { return Self{ value }; }

        static Self MakeFrom(const CharT* value) { return Self(value); }

        static Self MakeFrom(float value)
        {
            Self temp;
            temp.reserve(16);
            Toolset::FromFloat(value, temp.data(), temp.capacity());
            temp._size = Toolset::Length(temp.data());
            return temp;
        }

        static Self MakeFrom(double value)
        {
            Self temp;
            temp.reserve(32);
            Toolset::FromDouble(value, temp.data(), temp.capacity());
            temp._size = Toolset::Length(temp.data());
            return temp;
        }

        static Self MakeFrom(const std::filesystem::path& value)
        {
            Self temp;
            temp.reserve(1024);
            Toolset::FromStdFilesystemPath(value, temp.data(), temp.capacity());
            temp._size = Toolset::Length(temp.data());
            return temp;
        }

        template<class T>
        [[nodiscard]] std::enable_if_t<Utils::is_non_narrowing_convertible_v<T, uint64_t> && std::is_unsigned_v<T>, T> convertTo() const noexcept
        {
            if (!isEmpty())
            {
                return static_cast<T>(Toolset::ToUInt64(_string));
            }
            Assert("Impossible to work with nullptr string.");
            return {};
        }

        template<class T>
        [[nodiscard]] std::enable_if_t<Utils::is_non_narrowing_convertible_v<T, int64_t> && std::is_signed_v<T>, T> convertTo() const noexcept
        {
            if (!isEmpty())
            {
                return static_cast<T>(Toolset::ToInt64(_string));
            }
            Assert("Impossible to work with nullptr string.");
            return {};
        }

        template<class T>
        [[nodiscard]] std::enable_if_t<std::is_floating_point_v<T>, T> convertTo() const noexcept
        {
            if (!isEmpty())
            {
                return static_cast<T>(Toolset::ToDouble(_string));
            }
            Assert("Impossible to work with nullptr string.");
            return {};
        }

        template<IsFormattableType... T>
        [[nodiscard]] static Self Format(StdStringViewT str, const T&... args)
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
            (temp.regexReplace(static_cast<const CharT*>(expr), MakeFrom(args).toStdStringView()), ...);

            return temp;
        }

        [[nodiscard]] HashT makeHash() const noexcept
        {
            if (!isEmpty())
            {
                return std::hash<StdStringViewT>{}({ _string, _size });
            }
            return {};
        }

        Self& subStr(IndexT index, SizeT count = 0) noexcept
        {
            if (!isEmpty())
            {
                const SizeT finalCount = count == 0 ? _size - index : count - index;
                *this = std::move(Self(_string + index, finalCount));
            }

            return *this;
        }

        Self& trimStart(CharT ch) noexcept
        {
            if (!isEmpty())
            {
                SizeT offset = 0;
                while (offset < _size && _string[offset] == ch)
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

        Self& trimEnd(CharT ch) noexcept
        {
            if (!isEmpty())
            {
                SizeT count = 0;
                const CharT* stringEnd = _string + _size;
                while (count < _size && *--stringEnd == ch)
                {
                    ++count;
                }

                if (count != 0)
                {
                    resize(_size - count);
                }
            }

            return *this;
        }

        Self& trim(CharT ch) noexcept { return trimStart(ch).trimEnd(ch); }

        Self& toUpperCase() noexcept
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();
                for (IndexT i = 0; i < _size; ++i)
                {
                    _string[i] = static_cast<CharT>(Toolset::ToUpper(_string[i]));
                }
            }

            return *this;
        }

        Self& toLowerCase() noexcept
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();
                for (IndexT i = 0; i < _size; ++i)
                {
                    _string[i] = static_cast<CharT>(Toolset::ToLower(_string[i]));
                }
            }

            return *this;
        }

        Self& erase(IndexT index)
        {
            if (!isEmpty())
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

        Self& erase(IndexT from, IndexT to)
        {
            if (!isEmpty())
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

        Self& erase(Iterator iterator)
        {
            if (!isEmpty())
            {
                if (Verify(iterator._owner == this && iterator._data, "Was passed an invalid iterator"))
                {
                    return erase(iterator._data - _string);
                }
            }
            return *this;
        }

        Self& erase(Iterator from, Iterator to)
        {
            if (!isEmpty())
            {
                if (Verify(from._owner == this && from._data, "Was passed an invalid iterator 'from'") &&
                    Verify(to._owner == this && to._data, "Was passed an invalid iterator 'to'"))
                {
                    return erase(from._data - _string, to._data - _string);
                }
            }
            return *this;
        }

        Self& replaceFirst(StdStringViewT mainValue, StdStringViewT newValue) noexcept
        {
            if (!isEmpty())
            {
                if (auto* found = find(mainValue))
                {
                    auto temp = Self(_string, found - _string);
                    temp += newValue;
                    temp += found + mainValue.size();
                    *this = std::move(temp);
                }
            }

            return *this;
        }

        Self& replaceAll(StdStringViewT mainValue, StdStringViewT newValue) noexcept
        {
            if (!isEmpty())
            {
                int offset = 0;
                // TODO: optimize this code
                while (auto* found = find(mainValue, offset))
                {
                    offset = static_cast<int>(found - _string + newValue.size());
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
            return std::ranges::any_of(set,
                                       [ch](auto value)
                                       {
                                           return value == ch;
                                       });
        }

        /**
         * @param expr regex pattern
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param matchOptions corresponding to PCRE2 rules
         */
        template<class _T = CharType, class = std::enable_if_t<std::is_same_v<_T, char>>>
        [[nodiscard]] RegexMatch::MatchedData regexFind(StdStringViewT expr, uint64_t offset = 0, uint64_t limit = 0, uint32_t matchOptions = 0,
                                                        uint32_t compileOptions = 0) const
        {
            if (isEmpty() || expr.empty())
            {
                return {};
            }

            Core::RegexMatch regex(expr.data(), _string);
            regex.setOffset(offset);
            if (limit != 0)
            {
                regex.setLimit(limit);
            }
            regex.setMatchOptions(matchOptions);
            regex.setCompileOptions(compileOptions);

            if (regex.compile()) [[likely]]
            {
                return regex.match();
            }

            return {};
        }

        /**
         * @param expr regex pattern
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param matchOptions corresponding to PCRE2 rules
         */
        template<class _T = CharType, class = std::enable_if_t<std::is_same_v<_T, char>>>
        [[nodiscard]] RegexMatch::MatchedDataVector regexFindAll(StdStringViewT expr, uint64_t offset = 0, uint64_t limit = 0,
                                                                 uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            if (isEmpty() || expr.empty())
            {
                return {};
            }

            Core::RegexMatch regex(expr.data(), _string);
            regex.setOffset(offset);
            if (limit != 0)
            {
                regex.setLimit(limit);
            }
            regex.setMatchOptions(matchOptions);
            regex.setCompileOptions(compileOptions);

            if (regex.compile()) [[likely]]
            {
                return regex.matchAll();
            }

            return {};
        }

        /**
         * @param expr regex pattern
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param matchOptions corresponding to PCRE2 rules
         */
        template<class _T = CharType, class = std::enable_if_t<std::is_same_v<_T, char>>>
        [[nodiscard]] bool regexMatch(StdStringViewT expr, uint64_t offset = 0, uint64_t limit = 0, uint32_t matchOptions = 0,
                                      uint32_t compileOptions = 0) const
        {
            if (!isEmpty())
            {
                return regexFind(std::move(expr), offset, limit, matchOptions | PCRE2_ANCHORED, compileOptions).isMatched();
            }

            return false;
        }

        /**
         * @brief Will iterate over every match until the end.
         * @param expr regex pattern
         * @param callback can take a few function's type:
         * - void(MatchedData) - will iterate until the end.
         * - bool(MatchedData) - will iterate until 'true' is returned from the callback.
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param matchOptions corresponding to PCRE2 rules
         */
        template<class FuncT, class _T = CharType, class = std::enable_if_t<std::is_same_v<_T, char>>>
        void regexIterate(StdStringViewT expr, FuncT&& callback, uint64_t offset = 0, uint64_t limit = 0, uint32_t matchOptions = 0,
                          uint32_t compileOptions = 0) const
        {
            if (isEmpty() || expr.empty())
            {
                return;
            }

            Core::RegexMatch regex(expr.data(), _string);
            regex.setOffset(offset);
            if (limit != 0)
            {
                regex.setLimit(limit);
            }
            regex.setMatchOptions(matchOptions);
            regex.setCompileOptions(compileOptions);

            if (regex.compile()) [[likely]]
            {
                regex.iterateOverMatches(std::forward<decltype(callback)>(callback));
            }
        }

        /**
         * @param expr regex pattern
         * @param predictedScaleSize will scale future string allocation size based on
         * the programmer prediction. If new string will be bigger than needed - it will
         * pretend to UB. This function is using BaseString::Reserve - so final value will
         * be multiplied to BaseString::_capacityMultiplier
         * @param newValue new string for the replacement
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param replaceOptions corresponding to PCRE2 rules
         */
        template<class _T = CharType, class = std::enable_if_t<std::is_same_v<_T, char>>>
        bool regexReplace(StdStringViewT expr, StdStringViewT newValue, int predictedScaleSize = 2, uint64_t offset = 0, uint64_t limit = 0,
                          uint32_t replaceOptions = 0)
        {
            if (isEmpty() || expr.empty())
            {
                return false;
            }

            Core::RegexReplace regex(expr.data(), _string);
            regex.setOffset(offset);
            if (limit != 0)
            {
                regex.setLimit(limit);
            }
            regex.setReplaceOptions(replaceOptions);

            if (regex.compile()) [[likely]]
            {
                Self output;
                output.reserve(_size * predictedScaleSize);
                regex.setOutputString(output._string, output._capacity - 1);
                regex.setReplacementString(newValue.data());

                if (regex.replace())
                {
                    // recal size
                    output._size = 0;
                    while (output._string[output._size])
                    {
                        ++output._size;
                    }

                    const bool isChanged = output == *this;
                    *this = std::move(output);
                    return !isChanged;
                }
                return false;
            }

            return false;
        }

        /**
         * @param expr regex pattern
         * @param predictedScaleSize will scale future string allocation size based on
         * the programmer prediction. If new string will be bigger than needed - it will
         * pretend to UB. This function is using BaseString::Reserve - so final value will
         * be multiplied to BaseString::_capacityMultiplier
         * @param newValue new string for the replacement
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param replaceOptions corresponding to PCRE2 rules
         */
        template<class _T = CharType, class = std::enable_if_t<std::is_same_v<_T, char>>>
        bool regexReplaceAll(StdStringViewT expr, StdStringViewT newValue, int predictedScaleSize = 2, uint64_t offset = 0, uint64_t limit = 0,
                             uint32_t replaceOptions = 0)
        {
            return regexReplace(std::move(expr), std::move(newValue), predictedScaleSize, offset, limit, replaceOptions | PCRE2_SUBSTITUTE_GLOBAL);
        }

        [[nodiscard]] Self getCopyAsDynamic() const { return BaseString(_string, _size); }

        [[nodiscard]] Self operator+(CharT ch) const
        {
            auto tmp = *this;
            tmp += ch;
            return tmp;
        }
        [[nodiscard]] Self operator+(const Self& str) const
        {
            auto tmp = *this;
            tmp += str;
            return tmp;
        }

        Self& operator+=(CharT ch) { return push_back(ch); }
        Self& operator+=(StdStringViewT str) { return push_back(str); }

        Self& push_back(StdStringViewT str)
        {
            if (str.empty())
            {
                return *this;
            }

            const auto oldSize = _size;
            const auto finalSize = _size + str.size();
            if (finalSize >= _capacity)
            {
                // 32 is minimum size to optimize working with small strings
                reserve(finalSize < 32 ? 32 : finalSize);
            }

            if (_string) [[likely]]
            {
                memcpy_s(_string + oldSize, (_capacity - oldSize) * sizeof(CharT), str.data(), str.size() * sizeof(CharT));

                _string[finalSize] = 0;
                _size += str.size();
            }
            return *this;
        }

        Self& push_back(const CharT* str, const SizeT size)
        {
            if (str == nullptr || size == 0)
            {
                return *this;
            }

            const auto oldSize = _size;
            const auto finalSize = _size + size;
            if (finalSize >= _capacity)
            {
                // 32 is minimum size to optimize working with small strings
                reserve(finalSize < 32 ? 32 : finalSize);
            }

            memcpy_s(_string + oldSize, (_capacity - oldSize) * sizeof(CharT), str, size * sizeof(CharT));

            _string[finalSize] = 0;
            _size += size;

            return *this;
        }

        Self& push_back(CharT ch)
        {
            if (_size + 1 >= _capacity)
            {
                reserve(_size + 1);
            }

            _string[_size++] = ch;

            return *this;
        }

        Self& push_front(CharT ch) { return push_front(StdStringViewT(&ch, 1)); }

        Self& push_front(StdStringViewT str)
        {
            if (str.empty())
            {
                return *this;
            }

            const auto oldSize = _size;
            const auto finalSize = _size + str.size();
            if (finalSize >= _capacity)
            {
                reserve(finalSize);
            }

            for (int64_t i = oldSize; i >= 0; --i)
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

        Self& pop_back()
        {
            if (_size > 0)
            {
                tryToMakeAsDynamic();
                _string[--_size] = 0;
            }
            return *this;
        }

        Self& pop_front()
        {
            if (_size > 0)
            {
                tryToMakeAsDynamic();
                for (SizeT i = 1ll; i < _size; ++i)
                {
                    _string[i - 1ll] = _string[i];
                }
                _string[--_size] = 0;
            }
            return *this;
        }

        const Self& copyTo(CharT* dest, SizeT count, SizeT offset = 0) const
        {
            if (!isEmpty())
            {
                memcpy_s(dest, count * sizeof(CharT), _string + offset, (std::min)(_size - offset, count) * sizeof(CharT));
                dest[count] = 0;
            }
            return *this;
        }

        Self& shrink_to_fit() noexcept
        {
            if (isEmpty())
            {
                return *this;
            }

            const auto* oldString = _string;
            const auto capacity = _size + static_cast<SizeT>(1);

            if ((_string = new CharT[capacity]))
            {
                _capacity = capacity;
                memcpy_s(_string, _size * sizeof(CharT), oldString, _size * sizeof(CharT));
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

        [[nodiscard]] SizeT capacity() const noexcept { return _capacity; }

        Self& insert(Iterator iterator, const CharT* str, SizeT size = Settings::invalidSize) noexcept
        {
            Assert(iterator._owner == this);
            if (iterator._owner == this)
            {
                return insert(iterator._data - _string, str, size);
            }

            return *this;
        }

        Self& insert(int64_t pos, const CharT* str, SizeT size = Settings::invalidSize) noexcept
        {
            if (size == Settings::invalidSize)
            {
                size = Toolset::Length(str);
            }

            const auto oldSize = _size;
            const auto finalSize = _size + size;
            if (finalSize >= _capacity)
            {
                reserve(finalSize);
            }

            for (int64_t i = oldSize; i >= pos; --i)
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

        [[nodiscard]] bool isStatic() const noexcept { return _policy == StringPolicy::Static; }
        [[nodiscard]] bool isDynamic() const noexcept { return _policy == StringPolicy::Dynamic; }
        [[nodiscard, maybe_unused]] bool checkForPolicy(StringPolicy policy) const noexcept { return _policy == policy; }

        [[nodiscard]] Comparison compare(StdStringViewT other, const bool isIgnoreCase = false) const noexcept
        {
            if (isEmpty() || other.empty())
            {
                if (isEmpty() && other.empty())
                {
                    return Comparison::Equal;
                }

                return Comparison::None;
            }

            if (isIgnoreCase)
            {
                for (IndexT index = 0; index < other.size() && _string[index]; ++index)
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

        [[nodiscard]] const CharT* find(StdStringViewT other, uint64_t baseOffset = 0) const noexcept
        {
            if (isEmpty())
            {
                return nullptr;
            }

            if (other.empty())
            {
                return _string;
            }
            return Toolset::StrStr(_string + baseOffset, other.data());
        }

        [[nodiscard]] const CharT* reverseFind(StdStringViewT other, uint64_t baseOffset = 0, uint64_t limitOffset = 0) const noexcept
        {
            if (isEmpty())
            {
                return nullptr;
            }

            if (other.empty())
            {
                return _string;
            }

            return Toolset::ReverseStrStr(_string + baseOffset, other.data(), _string + size() - limitOffset);
        }

        [[nodiscard]] std::vector<const CharT*> findAll(StdStringViewT other) const noexcept
        {
            if (isEmpty() || other.empty())
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

        BaseString()
        {
            // 32 is minimum size to optimize working with small strings
            reserve(32);
        }

        template<class IterT>
        BaseString(IterT first, IterT last)
        {
            if (first != last)
            {
                constexpr const SizeT defaultCapacity = 16;
                reserve(defaultCapacity);
                for (; first != last; ++first)
                {
                    push_back(static_cast<CharT>(0));
                    std::char_traits<CharT>::assign(_string[_size - static_cast<SizeT>(1)], *first);
                }
            }
        }

        BaseString(const CharT* str, SizeT size = Settings::invalidSize)
        {
            if (str)
            {
                this->resize(size == Settings::invalidSize ? Toolset::Length(str) : size);
                memcpy_s(_string, _size * sizeof(CharT), str, _size * sizeof(CharT));
            }
        }

        explicit BaseString(StdStringViewT str)
            : BaseString(str.data(), str.size())
        {
        }

        BaseString(const Self& other) { *this = other; }

        explicit BaseString(SizeT reserveCount) { reserve(reserveCount); }

        Self& operator=(const CharT* str)
        {
            clear();
            const auto strSize = Toolset::Length(str);
            resize(strSize);
            memcpy_s(_string, _size * sizeof(CharT), str, strSize * sizeof(CharT));
            return *this;
        }

        Self& operator=(StdStringViewT other)
        {
            clear();
            resize(other.size());
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
                clear();
                resize(other._size, true);
                memcpy_s(_string, _size * sizeof(CharT), other._string, other._size * sizeof(CharT));
            }
            else if (other._policy == StringPolicy::Static)
            {
                clear();
                _policy = other._policy;
                _string = other._string;
                _size = other._size;
                _capacity = other._capacity;
            }
            else
            {
                clear();
            }

            return *this;
        }

        BaseString(Self&& other) noexcept { *this = std::move(other); }

        Self& operator=(Self&& other) noexcept
        {
            if (other._policy == StringPolicy::Dynamic)
            {
                clear();
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
                clear();
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
                clear();
            }

            return *this;
        }

        void clear()
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

        Self& reserve(const SizeT newSize, bool isIgnoreMultiplier = false)
        {
            const auto oldCapacity = _capacity;

            constexpr auto one = static_cast<SizeT>(1);
            const SizeT finalCapacity = newSize * (isIgnoreMultiplier ? one : _capacityMultiplier) + one;
            if (auto* newString = new CharT[finalCapacity]{})
            {
                if (_string)
                {
                    memcpy(newString, _string, (std::min)(finalCapacity, oldCapacity) * sizeof(CharT));
                }

                if (_policy == StringPolicy::Static)
                {
                    _string = nullptr;
                }
                else
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

        Self& resize(const SizeT newSize, bool isIgnoreMultiplier = false)
        {
            if (_string && newSize < _size && _policy != StringPolicy::Static)
            {
                _string[newSize] = 0;
            }
            else if (newSize > _size || _policy == StringPolicy::Static)
            {
                this->reserve(newSize, isIgnoreMultiplier);
            }
            // for empty init
            else if (newSize == 0 && _policy == StringPolicy::None)
            {
                this->reserve(1, isIgnoreMultiplier);
            }
            _size = newSize;

            return *this;
        }

        [[nodiscard]] BaseString<char> toASCII() const
        {
            BaseString<char> temp;
            temp.resize(_size);

            for (SizeT i = 0; i < _size; ++i)
            {
                temp[i] = static_cast<char>(_string[i]);
            }
            return temp;
        }

        ~BaseString() override { clear(); }

        // ============= Utils ===============
        static constexpr const CharT* GetLineSeparatorString(LineSeparator separator)
        {
            const CharT* sep = nullptr;

            if constexpr (std::is_same_v<CharT, char>)
            {
                if (separator == LineSeparator::LF)
                {
                    sep = "\n";
                }
                else if (separator == LineSeparator::CR)
                {
                    sep = "\r";
                }
                else if (separator == LineSeparator::LFCR)
                {
                    sep = "\n\r";
                }
                else if (separator == LineSeparator::CRLF)
                {
                    sep = "\r\n";
                }
                else
                {
                    Assert(false);
                    sep = "\n";
                }
            }
            else
            {
                if (separator == LineSeparator::LF)
                {
                    sep = L"\n";
                }
                else if (separator == LineSeparator::CR)
                {
                    sep = L"\r";
                }
                else if (separator == LineSeparator::LFCR)
                {
                    sep = L"\n\r";
                }
                else if (separator == LineSeparator::CRLF)
                {
                    sep = L"\r\n";
                }
                else
                {
                    Assert(false);
                    sep = L"\n";
                }
            }

            return sep;
        }

        static constexpr int32_t GetLineSeparatorStringSize(LineSeparator separator)
        {
            if (separator == LineSeparator::LF || separator == LineSeparator::CR)
            {
                return 1;
            }

            return 2;
        }

        static uint64_t GetLinesCountInText(const CharT* source, const CharT* end = nullptr, LineSeparator separator = LineSeparator::LF) noexcept
        {
            if (!Verify(source, "Impossible to calculate count of lines in the text, because was passed NULL pointer to the string."))
            {
                return 0;
            }

            if (end == nullptr)
            {
                end = source + Toolset::Length(source);
            }

            uint64_t count = 0;
            const auto* string = source;
            while ((string = FindNextLine(string, end, separator)))
            {
                ++count;
            }

            return ++count;
        }

        static const CharT* FindNextLine(const CharT* string, const CharT* end = nullptr, LineSeparator separator = LineSeparator::LF)
        {
            const auto* result = _StringToolset<CharT>::StrStr(string, GetLineSeparatorString(separator));
            if (result != nullptr)
            {
                if (end != nullptr && result >= end)
                {
                    return nullptr;
                }

                result += GetLineSeparatorStringSize(separator);
            }
            return result;
        }

        static CharT* FindNextLine(CharT* string, const CharT* end = nullptr, LineSeparator separator = LineSeparator::LF)
        {
            return const_cast<const CharT*>(FindNextLine(static_cast<const CharT*>(string), end, separator));
        }

        static const CharT* FindPrevLine(const CharT* begin, const CharT* end = nullptr, LineSeparator separator = LineSeparator::LF)
        {
            if (begin == nullptr)
            {
                return nullptr;
            }

            const auto* result = _StringToolset<CharT>::ReverseStrStr(begin, GetLineSeparatorString(separator), end);
            if (result != nullptr && result != begin)
            {
                result += GetLineSeparatorStringSize(separator);
            }

            return result;
        }

        /**
         * @brief Can take a functions of next types:
         * bool(String) - this function will work until it gets 'false' in return
         * void(String) - will iterate without stopping through all main string
         */
        template<class FuncT>
        void forEachByLine(FuncT&& callback, LineSeparator separator = LineSeparator::LF)
        {
            ForEachByLineImpl<false, FuncT>(this, std::forward<decltype(callback)>(callback), separator);
        }

        /**
         * @brief Can take a functions of next types:
         * bool(String) - this function will work until it gets 'false' in return
         * void(String) - will iterate without stopping through all main string
         */
        template<class FuncT>
        void forEachByLine(FuncT&& callback, LineSeparator separator = LineSeparator::LF) const
        {
            ForEachByLineImpl<true, FuncT>(this, std::forward<decltype(callback)>(callback), separator);
        }

    protected:
        explicit BaseString(StringDataReadOnlyT data)
            : _string{ data.str },
              _size{ data.size },
              _capacity{ data.size + static_cast<SizeT>(1) },
              _policy{ StringPolicy::Static }
        {
        }

        void tryToMakeAsDynamic()
        {
            if (_policy != StringPolicy::Dynamic && !isEmpty())
            {
                reserve(_size);
            }
        }

    protected:
        CharT* _string = nullptr;
        SizeT _size = 0;
        SizeT _capacity = 0;
        StringPolicy _policy = StringPolicy::None;
        static constexpr SizeT _capacityMultiplier = 2ull;

    private:
        // ================== PIMPLs =======================
        template<bool IsConst, class FuncT>
        static void ForEachByLineImpl(AdaptiveRawPtr<IsConst> base, FuncT&& callback, LineSeparator separator)
        {
            if (!base)
            {
                return;
            }

            auto* oldString = base->c_str();
            decltype(oldString) nextLine = nullptr;

            do
            {
                nextLine = FindNextLine(oldString, nullptr, separator);
                SizeT strSize = Settings::invalidSize;
                if (nextLine != nullptr)
                {
                    strSize = nextLine - oldString - GetLineSeparatorStringSize(separator);
                }

                Self temp(oldString, strSize);
                temp.trim(static_cast<CharT>('\n'));
                temp.trim(static_cast<CharT>('\r'));
                temp.trim(static_cast<CharT>('\n'));
                temp.trim(static_cast<CharT>('\r'));

                if constexpr (std::is_void_v<decltype(callback(temp))>)
                {
                    std::invoke(std::forward<decltype(callback)>(callback), std::move(temp));
                }
                else
                {
                    if (!std::invoke(std::forward<decltype(callback)>(callback), std::move(temp)))
                    {
                        return;
                    }
                }

                oldString = nextLine;
            } while (nextLine);
        }
    };

    template<class CharType>
    class StringFormatter : public BaseString<CharType>
    {
    public:
        using CharT = CharType;
        using Self = StringFormatter<CharT>;
        using String = BaseString<CharT>;
        using BaseString<CharT>::BaseString;

        template<IsFormattableType T>
        Self& operator<<(const T& value)
        {
            FormatFirst(value);
            return *this;
        }

    protected:
        template<IsFormattableType T>
        void FormatFirst(const T& arg)
        {
            const void* expr = nullptr;
            if constexpr (sizeof(CharT) == 1)
            {
                expr = "{}";
            }
            else
            {
                expr = L"{}";
            }
#ifdef UTILS_DEBUG
            if (!this->find(static_cast<const CharT*>(expr)) && this->_string)
            {
                std::stringstream ss;
                ss << "Can't find replace token '" << static_cast<const CharT*>(expr) << "' while formatting of string: " << this->_string
                   << std::endl;
                Assert(ss.str().c_str());
            }
#endif

            this->replaceFirst(static_cast<const CharT*>(expr), String::MakeFrom(arg));
        }
    };

    using StringAtom = BaseString<char>;
    using WStringAtom = BaseString<wchar_t>;
} // namespace Core

template<class CharType>
struct std::hash<Core::BaseString<CharType>>
{
    size_t operator()(const Core::BaseString<CharType>& x) const noexcept { return x.makeHash(); }
};

inline Core::BaseString<char> operator""_atom(const char* str, uint64_t size) noexcept
{
    return Core::BaseString<char>::Intern(str, size);
}

inline Core::BaseString<wchar_t> operator""_atom(const wchar_t* str, uint64_t size) noexcept
{
    return Core::BaseString<wchar_t>::Intern(str, size);
}

inline Core::BaseString<char> operator""_dyn(const char* str, uint64_t size) noexcept
{
    return { str, size };
}

inline Core::BaseString<wchar_t> operator""_dyn(const wchar_t* str, uint64_t size) noexcept
{
    return { str, size };
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

template<class CharType>
[[nodiscard]] Core::BaseString<CharType> operator+(typename Core::_StringToolset<CharType>::StdStringViewT str1,
                                                   const Core::BaseString<CharType>& str2)
{
    auto temp = Core::BaseString<CharType>(str1);
    temp += str2;
    return temp;
}

template<class CharType>
[[nodiscard]] Core::BaseString<CharType> operator+(CharType str1, const Core::BaseString<CharType>& str2)
{
    auto temp = Core::BaseString<CharType>(&str1, 1);
    temp += str2;
    return temp;
}

inline Core::StringFormatter<char> operator""_f(const char* str, uint64_t size)
{
    return { str, size };
}

inline Core::StringFormatter<wchar_t> operator""_f(const wchar_t* str, uint64_t size)
{
    return { str, size };
}
