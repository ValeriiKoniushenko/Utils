/*
 * MIT License
 *
 * Copyright (c) 2018-2025 Valerii Koniushenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "Core/AbstractIterators.h"
#include "Core/CommonEnums.h"
#include "Regex.h"
#include "Singleton.h"
#include "Utils/CopyableAndMoveableBehaviour.h"
#include "Utils/CrossString.h"
#include "Utils/TypeTraits.h"
#include "libassert/assert.hpp"

#include <algorithm>
#include <array>
#include <charconv>
#include <cstring>
#include <cwctype>
#include <filesystem>
#include <functional>
#include <optional>
#include <regex>
#include <type_traits>
#include <unordered_map>
#include <utility>

#if defined(UTILS_DEBUG)
    #include <sstream>
#endif

namespace Core
{

    class Iterator;

    template<class CharType>
    class BaseString;

#if defined(UTILS_DEBUG)
    class StringTracer : public StrictSingleton<StringTracer>
    {
        SINGLETONS_FRIEND(StringTracer)
    public:
        ~StringTracer() override;

        [[maybe_unused]] void addAtomRequest(std::string str);
        [[maybe_unused]] void addAtomComparisonRequest(std::string str);
        [[maybe_unused]] void addDynamicComparisonRequest(std::string str);
        [[maybe_unused]] void addChangedPolicyToDynamic(std::string str);

        [[maybe_unused]] void saveMetrics();

    private:
        std::unordered_map<std::string, int64_t> _atomRequests;
        std::unordered_map<std::string, int64_t> _atomCmpRequests;
        std::unordered_map<std::string, int64_t> _dynamicCmpRequests;
        std::unordered_map<std::string, int64_t> _changedPolicyToDynamic;
    };
#endif // defined(UTILS_DEBUG)

    template<class CharT>
    struct StringToolset
    {
        StringToolset() = delete;

        using StdStringT = std::basic_string<CharT>;
        using StdStringViewT = std::basic_string_view<CharT>;

        constexpr static uint32_t fnv1aPrime = 16777619u;
        constexpr static uint32_t fnv1aOffsetBias = 2166136261u;

        [[nodiscard]] static uint32_t Hash(const CharT* str)
        {
            const auto length = Length(str) + 1u;
            uint32_t hash = fnv1aOffsetBias;
            for (size_t i = 0; i < length; ++i)
            {
                hash ^= *str++;
                hash *= fnv1aPrime;
            }
            return hash;
        }
        [[nodiscard]] static uint32_t Hash(const CharT* str, std::size_t length)
        {
            uint32_t hash = fnv1aOffsetBias;
            for (size_t i = 0; i < length; ++i)
            {
                hash ^= *str++;
                hash *= fnv1aPrime;
            }
            return hash;
        }

        [[nodiscard]] static bool IsSpace(int ch)
        {
            if constexpr (sizeof(CharT) == 1)
            {
                return static_cast<bool>(isspace(ch));
            }
            else
            {
                return static_cast<bool>(std::iswspace(ch));
            }
        }
        [[nodiscard]] static std::size_t Length(const CharT* string) noexcept
        {
            if constexpr (sizeof(CharT) == 1)
            {
                return static_cast<std::size_t>(strlen(string));
            }
            else
            {
                return static_cast<std::size_t>(wcslen(string));
            }
        }

        [[nodiscard]] static CharT* StrTok(CharT* string, const CharT* delimiter, CharT*& context) noexcept
        {
            if constexpr (sizeof(CharT) == 1)
            {
                return strtok_s(string, delimiter, &context);
            }
            else
            {
                return wcstok_s(string, delimiter, &context);
            }
        }

        [[nodiscard]] static CharT* StrStr(CharT* mainString, const CharT* subString) noexcept
        {
            if constexpr (sizeof(CharT) == 1)
            {
                return strstr(mainString, subString);
            }
            else
            {
                return wcsstr(mainString, subString);
            }
        }
        [[nodiscard]] static const CharT* StrStr(const CharT* mainString, const CharT* subString) noexcept
        {
            if constexpr (sizeof(CharT) == 1)
            {
                return strstr(mainString, subString);
            }
            else
            {
                return wcsstr(mainString, subString);
            }
        }
        [[nodiscard]] static const CharT* StrIStr(const CharT* mainString, const CharT* subString)
        {
            const CharT* p1 = mainString;
            const CharT* p2 = subString;
            const CharT* r = *p2 == 0 ? mainString : nullptr;

            while (*p1 != 0 && *p2 != 0)
            {
                if (ToLower(*p1) == ToLower(*p2))
                {
                    if (r == nullptr)
                    {
                        r = p1;
                    }
                    ++p2;
                }
                else
                {
                    p2 = subString;
                    if (r != nullptr)
                    {
                        p1 = r + 1;
                    }

                    if (ToLower(*p1) == ToLower(*p2))
                    {
                        r = p1;
                        ++p2;
                    }
                    else
                    {
                        r = nullptr;
                    }
                }
                ++p1;
            }

            return *p2 == 0 ? r : nullptr;
        }
        [[nodiscard]] static CharT* StrIStr(CharT* mainString, const CharT* subString)
        {
            return const_cast<CharT*>(StrIStr(static_cast<const CharT*>(mainString), subString));
        }

        [[nodiscard]] static int ToUpper(const CharT ch) noexcept
        {
            if constexpr (sizeof(CharT) == 1)
            {
                return toupper(ch);
            }
            else
            {
                return towupper(ch);
            }
        };
        [[nodiscard]] static int ToLower(const CharT ch) noexcept
        {
            if constexpr (sizeof(CharT) == 1)
            {
                return tolower(ch);
            }
            else
            {
                return towlower(ch);
            }
        };

        [[nodiscard]] static Comparison Cmp(const CharT* str1, const CharT* str2) noexcept
        {
            int result = 0;
            if constexpr (sizeof(CharT) == 1)
            {
                result = strcmp(str1, str2);
            }
            else
            {
                result = wcscmp(str1, str2);
            }

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

            if (*substr == 0)
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
        static CharT* ReverseStrStr(CharT* string, const CharT* substr, const CharT* end = nullptr)
        {
            return const_cast<CharT*>(ReverseStrStr(static_cast<const CharT*>(string), substr, end));
        }

        template<class T>
        [[nodiscard]] static T FromCStringTo(const CharT* str)
        {
            if (!str)
            {
                return {};
            }

            // =========== FLOATING POINT =============
            if constexpr (std::is_floating_point_v<T>)
            {
                // char
                if constexpr (sizeof(CharT) == 1)
                {
                    return std::is_same_v<T, float> ? std::stof(str) : std::stod(str);
                }
                // wchar_t
                else
                {
                    CharT* end = nullptr;
                    const auto r = std::is_same_v<T, float> ? std::wcstof(str, &end) : std::wcstod(str, &end);
                    if (end == str)
                    {
                        throw std::invalid_argument("Can't convert wide string to the floating type");
                    }
                    return r;
                }
            }
            // =========== INTEGRAL =============
            else if constexpr (std::is_integral_v<T>)
            {
                // >>> non-narrow int32 <<<
                if constexpr (Utils::IsNonNarrowingConvertibleV<std::make_unsigned_t<T>, uint32_t>)
                {
                    // char
                    if constexpr (sizeof(CharT) == 1)
                    {
                        return static_cast<T>(std::is_signed_v<T> ? std::stoi(str) : std::stoul(str));
                    }
                    // wchar_t
                    else
                    {
                        CharT* end = nullptr;
                        const auto r = static_cast<T>(std::is_signed_v<T> ? std::wcstol(str, &end, 10) : std::wcstoul(str, &end, 10));
                        if (end == str)
                        {
                            throw std::invalid_argument("Can't convert wide string to the [u]int32");
                        }
                        return r;
                    }
                }
                // >>> non-narrow int64 <<<
                else if constexpr (Utils::IsNonNarrowingConvertibleV<std::make_unsigned_t<T>, std::size_t>)
                {
                    // char
                    if constexpr (sizeof(CharT) == 1)
                    {
                        return static_cast<T>(std::is_signed_v<T> ? std::stoll(str) : std::stoull(str));
                    }
                    // wchar_t
                    else
                    {
                        CharT* end = nullptr;
                        const auto r = static_cast<T>(std::is_signed_v<T> ? std::wcstoll(str, &end, 10) : std::wcstoull(str, &end, 10));
                        if (end == str)
                        {
                            throw std::invalid_argument("Can't convert wide string to the [u]int64");
                        }
                        return r;
                    }
                }
                else
                {
                    static_assert(false, "Can't determine integer type");
                }
            }
            else
            {
                static_assert(false, "Unsupported type. Can't convert from string to your T");
            }
            return {};
        }
    };

    template<class CharT>
    struct StringDataReadOnly
    {
        constexpr static std::size_t invalidSize = ~static_cast<std::size_t>(0);

        CharT* str = nullptr;
        std::size_t size = invalidSize;
    };

    template<class CharT>
    struct StringData
    {
        using Toolset = StringToolset<CharT>;

        StringData(std::unique_ptr<CharT[]>&& ptr, std::size_t newSize)
            : str{ std::move(ptr) },
              size{ newSize }
        {
        }

        [[nodiscard]] StringDataReadOnly<CharT> toReadOnly() noexcept { return { str.get(), size }; }

        [[nodiscard]] bool operator<(const StringData& other) const { return Toolset::Cmp(str.get(), other.str.get()) == Comparison::Less; }

        [[nodiscard]] bool operator==(const StringData& other) const { return Toolset::Cmp(str.get(), other.str.get()) == Comparison::Equal; }

        std::unique_ptr<CharT[]> str;
        std::size_t size = StringDataReadOnly<CharT>::invalidSize;
    };

    template<class CharType>
    class StringPool : public StrictSingleton<StringPool<CharType>>
    {
        SINGLETONS_FRIEND_NO_CNSTR(StringPool<CharType>)
    public:
        using CharT = CharType;
        using Toolset = StringToolset<CharT>;
        using StdStringViewT = typename Toolset::StdStringViewT;
        using StringDataT = StringData<CharT>;
        using StringDataReadOnlyT = StringDataReadOnly<CharT>;

    public:
        [[nodiscard]] StringDataReadOnlyT intern(const CharT* string, std::size_t size)
        {
#if defined(UTILS_DEBUG)
            if constexpr (sizeof(CharT) == 1)
            {
                StringTracer::instance().addAtomRequest(std::string(string));
            }
#endif
            const auto currentHash = Toolset::Hash(string, size);
            auto it = _strings.find(currentHash);
            if (it != _strings.end())
            {
                return it->second.toReadOnly();
            }

            auto ptr = std::make_unique<CharT[]>(size + 1);
            auto* addr = ptr.get();
            memcpy(ptr.get(), string, (size + 1) * sizeof(CharT));
            _strings.emplace(currentHash, StringDataT{ std::move(ptr), size });

            return StringDataReadOnlyT{ addr, size };
        }

        [[nodiscard]] bool isStatic(const CharT* string, std::size_t size) { return _strings.contains(Toolset::Hash(string, size)); }

        void clear()
        {
            for (auto& pair : _strings)
            {
                pair.second.clear();
            }
            _strings.clear();
        }

        std::unordered_map<std::size_t, StringDataT>& _raw() { return _strings; }

    protected:
        StringPool()
        {
#if defined(UTILS_STRING_POOL_SIZE) && UTILS_STRING_POOL_SIZE > 0
            _strings.reserve(UTILS_STRING_POOL_SIZE);
#else
            _strings.reserve(3000);
#endif // defined(UTILS_STRING_POOL_SIZE)
        }

    private:
        std::unordered_map<std::size_t, StringDataT> _strings;
    };

    template<class CharType>
    class BaseString
    {
    public:
        using CharT = CharType;
        using Self = BaseString<CharT>;
        using Toolset = StringToolset<CharT>;
        using StdStringT = typename Toolset::StdStringT;
        using StdStringViewT = typename Toolset::StdStringViewT;
        using StringDataReadOnlyT = StringDataReadOnly<CharT>;

        using value_type = CharT;
        using pointer = value_type*;
        using difference_type = int64_t;

        template<bool IsConst>
        using AdaptiveRawPtr = std::conditional_t<IsConst, const Self, Self>*;

        constexpr static std::size_t invalidSize = StringDataReadOnlyT::invalidSize;
        constexpr static std::size_t minAllocationSize = 32u;

        enum class LineSeparator : uint8_t
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

            void swap(Self& other) noexcept override
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
                    DEBUG_ASSERT("Impossible to compare two iterators. Some iterator is invalid");
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

                DEBUG_ASSERT("Impossible to compare two iterators. Was get some error");
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
        [[nodiscard]] static Self Intern(const CharT* newString)
        {
            auto& pool = StringPool<CharT>::instance();
            return Self{ pool.intern(newString, Toolset::Length(newString)) };
        }

        /**
         * @brief This function will use the provided string as a static string
         */
        [[nodiscard]] static Self Intern(const CharT* newString, std::size_t size)
        {
            auto& pool = StringPool<CharT>::instance();
            return Self{ pool.intern(newString, size) };
        }

        /**
         * @brief This function will use the provided string as a static string
         */
        [[nodiscard]] static Self Intern(StdStringViewT string)
        {
            auto& pool = StringPool<CharT>::instance();
            return Self{ pool.intern(string.data(), string.size()) };
        }

        [[nodiscard]] std::size_t size() const noexcept { return _size; }
        [[nodiscard]] std::size_t byteSize() const noexcept { return _size * sizeof(CharT); }
        [[nodiscard]] std::size_t length() const noexcept { return _size; }
        [[nodiscard]] bool isEmpty() const noexcept { return _string == nullptr || _size == 0; }
        [[nodiscard]] explicit operator const CharT*() const noexcept { return _string; }
        [[nodiscard]] operator StdStringViewT() const noexcept { return toStdStringView(); }
        [[nodiscard]] CharT& operator[](std::size_t index) noexcept { return _string[index]; }
        [[nodiscard]] CharT operator[](std::size_t index) const noexcept { return _string[index]; }

        [[nodiscard]] bool operator==(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                return isEmpty() && other.isEmpty();
            }

            if (isStatic() && other.isStatic())
            {
#if defined(UTILS_DEBUG)
                if constexpr (sizeof(CharT) == 1)
                {
                    StringTracer::instance().addAtomComparisonRequest(std::string(other.c_str()));
                }
#endif
                return _string == other._string;
            }

#if defined(UTILS_DEBUG)
            if constexpr (sizeof(CharT) == 1)
            {
                StringTracer::instance().addDynamicComparisonRequest(std::string(other.c_str()));
            }
#endif

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
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other._string) == Comparison::Greater;
        }

        [[nodiscard]] bool operator>=(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Greater || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator<(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other._string) == Comparison::Less;
        }

        [[nodiscard]] bool operator<=(const Self& other) const
        {
            if (isEmpty() || other.isEmpty())
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other._string);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                return ((_string && _string[0] == 0) || _string == nullptr) && ((other && other[0] == 0) || other == nullptr);
            }
            return Toolset::Cmp(_string, other) == Comparison::Equal;
        }

        [[nodiscard]] bool operator!=(const CharT* other) const { return !operator==(other); }

        [[nodiscard]] bool operator>(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Greater;
        }

        [[nodiscard]] bool operator>=(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Greater || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator<(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }
            return Toolset::Cmp(_string, other) == Comparison::Less;
        }

        [[nodiscard]] bool operator<=(const CharT* other) const
        {
            if (isEmpty() || !other)
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }
            const auto result = Toolset::Cmp(_string, other);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                return ((_string && _string[0] == 0) || _string == nullptr) && other.empty();
            }
            return *this == other.data();
        }

        [[nodiscard]] bool operator>(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                return _size > other.size();
            }
            return *this > other.data();
        }

        [[nodiscard]] bool operator>=(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                return _size >= other.size();
            }
            return *this >= other.data();
        }

        [[nodiscard]] bool operator<(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                return _size < other.size();
            }
            return *this < other.data();
        }

        [[nodiscard]] bool operator<=(StdStringViewT other) const
        {
            if (isEmpty() || other.empty())
            {
                return _size <= other.size();
            }
            return *this <= other.data();
        }

        [[nodiscard]] bool operator!() const noexcept { return isEmpty(); }
        [[nodiscard]] explicit operator bool() const noexcept { return !isEmpty(); }

        [[nodiscard]] CharT front() const
        {
            if (isEmpty())
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }

            return _string[0];
        }

        [[nodiscard]] CharT back() const
        {
            if (isEmpty())
            {
                DEBUG_ASSERT("Impossible to work with nullptr string.");
                return {};
            }

            return _string[_size - 1];
        }

        [[nodiscard]] StdStringViewT toStdStringView() const
        {
            if (isEmpty())
            {
                return {};
            }

            return { _string, _size };
        }

        [[nodiscard]] StdStringT toStdString() const
        {
            if (isEmpty())
            {
                return {};
            }

            return { _string ? _string : "", _size };
        }

        [[nodiscard]] CharT at(std::size_t index) const
        {
            if (!DEBUG_ASSERT_VAL(!isEmpty() || _size < index, "Impossible to work with nullptr string. or invalid index."))
            {
                return {};
            }

            return _string[index];
        }

        [[nodiscard]] CharT safeAt(std::size_t index) const
        {
            if (!DEBUG_ASSERT_VAL(!isEmpty() || _size < index, "Impossible to work with nullptr string. or invalid index."))
            {
                return {};
            }

            return _string[index];
        }

        [[nodiscard]] CharT& at(std::size_t index)
        {
            DEBUG_ASSERT(!isEmpty() || _size < index, "Impossible to work with nullptr string. or invalid index.");
            tryToMakeAsDynamic();
            return _string[index];
        }

        [[nodiscard]] const CharT* c_str() const noexcept { return _string; }

        [[nodiscard]] const CharT* data() const noexcept { return _string; }

        [[nodiscard]] CharT* data()
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

        template<class _T>
        static Self MakeFrom(const _T& value)
        {
            using T = std::remove_reference_t<std::remove_cv_t<_T>>;

            if constexpr (std::is_same_v<T, char>)
            {
                Self temp;
                temp.resize(1);
                temp.at(0) = value;
                return temp;
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                if constexpr (sizeof(CharT) == 1)
                {
                    return value ? Self{ "true" } : Self{ "false" };
                }
                else
                {
                    return value ? Self{ L"true" } : Self{ L"false" };
                }
            }
            else if constexpr (std::is_same_v<T, Self>)
            {
                return Self{ value.c_str(), value.size() };
            }
            else if constexpr (std::is_same_v<T, std::basic_string<CharT>>)
            {
                return Self{ value.c_str(), value.size() };
            }
            else if constexpr (std::is_same_v<T, std::basic_string_view<CharT>>)
            {
                return Self{ value.data(), value.size() };
            }
            else if constexpr (std::is_same_v<T, std::filesystem::path>)
            {
                auto temp = value.template generic_string<CharT>();
                return Self{ temp.c_str(), temp.size() };
            }
            else
            {
                static std::array<char, 4096> buffer{};
                memset(buffer.data(), 0, buffer.size() * sizeof(buffer[0]));
                Self temp;

                if (auto result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value); result.ec != std::errc())
                {
                    DEBUG_ASSERT(false, std::make_error_code(result.ec).message().c_str());
                    return temp;
                }

                if constexpr (sizeof(CharT) == 1)
                {
                    temp = buffer.data();
                }
                else
                {
                    for (std::size_t i = 0; i < buffer.size() && buffer[i] != '\0'; ++i)
                    {
                        temp.pushBack(buffer[i]);
                    }
                }

                return temp;
            }

            return {};
        }
        static Self MakeFrom(const CharT* value) { return { value }; }
        static Self MakeFrom(CharT* value) { return { value }; }

        template<class T>
        [[nodiscard]] T convertTo() const
        {
            return Toolset::template FromCStringTo<T>(_string);
        }

        template<class... T>
        [[nodiscard]] static Self Format(StdStringViewT str, const T&... args)
        {
            Self temp(str);
            const void* expr = nullptr;
            if constexpr (sizeof(CharT) == 1)
            {
                expr = "{}";
            }
            else
            {
                expr = L"{}";
            }
            (temp.replaceFirst(static_cast<const CharT*>(expr), MakeFrom(args).toStdStringView()), ...);

            return temp;
        }

        [[nodiscard]] std::size_t makeHash() const noexcept
        {
            if (!isEmpty())
            {
                return Toolset::Hash(_string, _size);
            }
            return {};
        }

        void subStr(std::size_t index, std::size_t count = 0)
        {
            if (!isEmpty())
            {
                const std::size_t finalCount = count == 0 ? _size - index : count - index;
                *this = std::move(Self(_string + index, finalCount));
            }
        }

        void trimStart(CharT ch)
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();
                std::size_t offset = 0;
                while (offset < _size && _string[offset] == ch)
                {
                    ++offset;
                }
                if (offset != 0)
                {
                    *this = std::move(Self(_string + offset, _size - offset));
                }
            }
        }

        void trimEnd(CharT ch)
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();
                std::size_t count = 0;
                while (count < _size && _string[_size - count - 1u] == ch)
                {
                    ++count;
                }

                if (count != 0)
                {
                    resize(_size - count);
                }
            }
        }

        void trim(CharT ch)
        {
            trimStart(ch);
            trimEnd(ch);
        }

        void toUpperCase()
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();
                for (std::size_t i = 0; i < _size; ++i)
                {
                    _string[i] = static_cast<CharT>(Toolset::ToUpper(_string[i]));
                }
            }
        }

        void toLowerCase()
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();
                for (std::size_t i = 0; i < _size; ++i)
                {
                    _string[i] = static_cast<CharT>(Toolset::ToLower(_string[i]));
                }
            }
        }

        void erase(std::size_t index)
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();

                if (!DEBUG_ASSERT_VAL(index < _size, "Invalid index")) [[unlikely]]
                {
                    return;
                }

                Self temp(_string, index);
                temp += _string + index + 1;
                *this = std::move(temp);
            }
        }

        void erase(std::size_t from, std::size_t to)
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();

                if (!DEBUG_ASSERT_VAL(from < _size && to < _size, "Invalid index")) [[unlikely]]
                {
                    return;
                }

                Self temp(_string, from);
                temp += _string + to + 1;
                *this = std::move(temp);
            }
        }

        void erase(Iterator iterator)
        {
            if (!isEmpty())
            {
                if (DEBUG_ASSERT_VAL(iterator._owner == this && iterator._data, "Was passed an invalid iterator"))
                {
                    erase(iterator._data - _string);
                }
            }
        }

        void erase(Iterator from, Iterator to)
        {
            if (!isEmpty())
            {
                if (DEBUG_ASSERT_VAL(from._owner == this && from._data, "Was passed an invalid iterator 'from'") &&
                    DEBUG_ASSERT_VAL(to._owner == this && to._data, "Was passed an invalid iterator 'to'"))
                {
                    erase(from._data - _string, to._data - _string);
                }
            }
        }

        void replaceFirst(StdStringViewT mainValue, StdStringViewT newValue) noexcept
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();
                if (auto* found = find(mainValue))
                {
                    auto temp = Self(_string, found - _string);
                    temp += newValue;
                    temp += found + mainValue.size();
                    *this = std::move(temp);
                }
            }
        }

        void replaceAll(StdStringViewT mainValue, StdStringViewT newValue) noexcept
        {
            if (!isEmpty())
            {
                tryToMakeAsDynamic();
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
         * @param compileOptions perl compile options
         */
        [[nodiscard]] RegexMatch::MatchedData regexFind(const char* expr, std::size_t offset = 0, std::size_t limit = 0, uint32_t matchOptions = 0,
                                                        uint32_t compileOptions = 0) const
        {
            if (isEmpty() || !expr)
            {
                return {};
            }

            Core::RegexMatch regex(expr, _string);
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
        [[nodiscard]] RegexMatch::MatchedData regexFind(std::basic_string_view<char> expr, std::size_t offset = 0, std::size_t limit = 0,
                                                        uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            return regexFind(expr.data(), offset, limit, matchOptions, compileOptions);
        }
        [[nodiscard]] RegexMatch::MatchedData regexFind(const std::basic_string<char>& expr, std::size_t offset = 0, std::size_t limit = 0,
                                                        uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            return regexFind(expr.c_str(), offset, limit, matchOptions, compileOptions);
        }
        [[nodiscard]] RegexMatch::MatchedData regexFind(const BaseString<char>& expr, std::size_t offset = 0, std::size_t limit = 0,
                                                        uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            return regexFind(expr.c_str(), offset, limit, matchOptions, compileOptions);
        }

        /**
         * @param expr regex pattern
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param matchOptions corresponding to PCRE2 rules
         * @param compileOptions PCRE regex compile options
         */
        [[nodiscard]] RegexMatch::MatchedDataVector regexFindAll(const char* expr, std::size_t offset = 0, std::size_t limit = 0,
                                                                 uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            if (isEmpty() || !expr)
            {
                return {};
            }

            RegexMatch regex(expr, _string);
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
        [[nodiscard]] RegexMatch::MatchedDataVector regexFindAll(std::basic_string_view<char> expr, std::size_t offset = 0, std::size_t limit = 0,
                                                                 uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            return regexFindAll(expr.data(), offset, limit, matchOptions, 0);
        }
        [[nodiscard]] RegexMatch::MatchedDataVector regexFindAll(const std::basic_string<char>& expr, std::size_t offset = 0, std::size_t limit = 0,
                                                                 uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            return regexFindAll(expr.c_str(), offset, limit, matchOptions, 0);
        }
        [[nodiscard]] RegexMatch::MatchedDataVector regexFindAll(const BaseString<char>& expr, std::size_t offset = 0, std::size_t limit = 0,
                                                                 uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            return regexFindAll(expr.c_str(), offset, limit, matchOptions, 0);
        }

        /**
         * @param expr regex pattern
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param matchOptions corresponding to PCRE2 rules
         * @param compileOptions PCRE2 regex compile options
         */
        [[nodiscard]] bool regexMatch(const char* expr, std::size_t offset = 0, std::size_t limit = 0, uint32_t matchOptions = 0,
                                      uint32_t compileOptions = 0) const
        {
            if (!isEmpty())
            {
                return regexFind(expr, offset, limit, matchOptions | PCRE2_ANCHORED, compileOptions).isMatched();
            }

            return false;
        }
        [[nodiscard]] bool regexMatch(std::basic_string_view<char> expr, std::size_t offset = 0, std::size_t limit = 0, uint32_t matchOptions = 0,
                                      uint32_t compileOptions = 0) const
        {
            return regexMatch(expr.data(), offset, limit, matchOptions, compileOptions);
        }
        [[nodiscard]] bool regexMatch(const std::basic_string<char>& expr, std::size_t offset = 0, std::size_t limit = 0, uint32_t matchOptions = 0,
                                      uint32_t compileOptions = 0) const
        {
            return regexMatch(expr.c_str(), offset, limit, matchOptions, compileOptions);
        }
        [[nodiscard]] bool regexMatch(const BaseString<char>& expr, std::size_t offset = 0, std::size_t limit = 0, uint32_t matchOptions = 0,
                                      uint32_t compileOptions = 0) const
        {
            return regexMatch(expr.c_str(), offset, limit, matchOptions, compileOptions);
        }

        /**
         * @brief Will iterate over every match until the end.
         * @param expr regex pattern
         * @param callback can take a few functions type:
         * - void(MatchedData) - will iterate until the end.
         * - bool(MatchedData) - will iterate until 'true' is returned from the callback.
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param matchOptions corresponding to PCRE2 rules
         */
        template<class FuncT>
        void regexIterate(const char* expr, FuncT&& callback, std::size_t offset = 0, std::size_t limit = 0, uint32_t matchOptions = 0,
                          uint32_t compileOptions = 0) const
        {
            if (isEmpty() || !expr)
            {
                return;
            }

            Core::RegexMatch regex(expr, _string);
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
        template<class FuncT>
        void regexIterate(std::basic_string_view<char> expr, FuncT&& callback, std::size_t offset = 0, std::size_t limit = 0,
                          uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            regexIterate(expr.data(), std::forward<decltype(callback)>(callback), offset, limit, matchOptions, compileOptions);
        }
        template<class FuncT>
        void regexIterate(const std::basic_string<char>& expr, FuncT&& callback, std::size_t offset = 0, std::size_t limit = 0,
                          uint32_t matchOptions = 0, uint32_t compileOptions = 0) const
        {
            regexIterate(expr.c_str(), std::forward<decltype(callback)>(callback), offset, limit, matchOptions, compileOptions);
        }
        template<class FuncT>
        void regexIterate(const BaseString<char>& expr, FuncT&& callback, std::size_t offset = 0, std::size_t limit = 0, uint32_t matchOptions = 0,
                          uint32_t compileOptions = 0) const
        {
            regexIterate(expr.c_str(), std::forward<decltype(callback)>(callback), offset, limit, matchOptions, compileOptions);
        }

        /**
         * @param expr regex pattern
         * @param predictedScaleSize will scale future string allocation size based on
         * the programmer prediction. If the new string is bigger than needed - it will
         * pretend to UB. This function is using BaseString::Reserve - so the final value will
         * be multiplied to BaseString::_capacityMultiplier
         * @param newValue new string for the replacement
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param replaceOptions corresponding to PCRE2 rules
         */
        bool regexReplace(const char* expr, StdStringViewT newValue, int predictedScaleSize = 2, std::size_t offset = 0, std::size_t limit = 0,
                          uint32_t replaceOptions = 0)
        {
            if (isEmpty() || !expr)
            {
                return false;
            }

            tryToMakeAsDynamic();

            Core::RegexReplace regex(expr, _string);
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
        bool regexReplace(std::basic_string_view<char> expr, StdStringViewT newValue, int predictedScaleSize = 2, std::size_t offset = 0,
                          std::size_t limit = 0, uint32_t replaceOptions = 0)
        {
            return regexReplace(expr.data(), newValue, predictedScaleSize, offset, limit, replaceOptions);
        }
        bool regexReplace(const std::basic_string<char>& expr, StdStringViewT newValue, int predictedScaleSize = 2, std::size_t offset = 0,
                          std::size_t limit = 0, uint32_t replaceOptions = 0)
        {
            return regexReplace(expr.c_str(), newValue, predictedScaleSize, offset, limit, replaceOptions);
        }
        bool regexReplace(const BaseString<char>& expr, StdStringViewT newValue, int predictedScaleSize = 2, std::size_t offset = 0,
                          std::size_t limit = 0, uint32_t replaceOptions = 0)
        {
            return regexReplace(expr.c_str(), newValue, predictedScaleSize, offset, limit, replaceOptions);
        }

        /**
         * @param expr regex pattern
         * @param predictedScaleSize will scale future string allocation size based on
         * the programmer prediction. If the new string is bigger than needed - it will
         * pretend to UB. This function is using BaseString::Reserve - so the final value will
         * be multiplied to BaseString::_capacityMultiplier
         * @param newValue new string for the replacement
         * @param offset from the start of the string
         * @param limit string size for searching of the matches
         * @param replaceOptions corresponding to PCRE2 rules
         */
        bool regexReplaceAll(const char* expr, StdStringViewT newValue, int predictedScaleSize = 2, std::size_t offset = 0, std::size_t limit = 0,
                             uint32_t replaceOptions = 0)
        {
            return regexReplace(expr, std::move(newValue), predictedScaleSize, offset, limit, replaceOptions | PCRE2_SUBSTITUTE_GLOBAL);
        }
        bool regexReplaceAll(std::basic_string_view<char> expr, StdStringViewT newValue, int predictedScaleSize = 2, std::size_t offset = 0,
                             std::size_t limit = 0, uint32_t replaceOptions = 0)
        {
            return regexReplaceAll(expr.data(), std::move(newValue), predictedScaleSize, offset, limit, replaceOptions | PCRE2_SUBSTITUTE_GLOBAL);
        }
        bool regexReplaceAll(const std::basic_string<char>& expr, StdStringViewT newValue, int predictedScaleSize = 2, std::size_t offset = 0,
                             std::size_t limit = 0, uint32_t replaceOptions = 0)
        {
            return regexReplaceAll(expr.c_str(), std::move(newValue), predictedScaleSize, offset, limit, replaceOptions | PCRE2_SUBSTITUTE_GLOBAL);
        }
        bool regexReplaceAll(const BaseString<char>& expr, StdStringViewT newValue, int predictedScaleSize = 2, std::size_t offset = 0,
                             std::size_t limit = 0, uint32_t replaceOptions = 0)
        {
            return regexReplaceAll(expr.c_str(), std::move(newValue), predictedScaleSize, offset, limit, replaceOptions | PCRE2_SUBSTITUTE_GLOBAL);
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

        Self& operator+=(CharT ch)
        {
            pushBack(ch);
            return *this;
        }

        Self& operator+=(StdStringViewT str)
        {
            pushBack(str);
            return *this;
        }

        void pushBack(StdStringViewT str) { pushBack(str.data(), str.size()); }

        void pushBack(CharT ch) { pushBack(&ch, 1); }

        void pushBack(const CharT* str, std::size_t size)
        {
            if (str == nullptr) [[unlikely]]
            {
                return;
            }
            tryToMakeAsDynamic();

            if (size == 0)
            {
                size = Toolset::Length(str);
            }

            const auto oldSize = _size;
            const auto finalSize = _size + size;
            if (finalSize >= _capacity)
            {
                reserve(finalSize * _capacityMultiplier);
            }

            memcpy_s(_string + oldSize, (_capacity - oldSize) * sizeof(CharT), str, size * sizeof(CharT));
            _size += size;
            _string[_size] = 0;
        }

        void pushFront(CharT ch) { return pushFront(&ch, 1); }

        void pushFront(StdStringViewT str) { return pushFront(str.data(), str.size()); }

        void pushFront(const CharT* str, std::size_t size)
        {
            if (str == nullptr)
            {
                return;
            }

            tryToMakeAsDynamic();

            if (size == 0)
            {
                size = Toolset::Length(str);
            }

            const auto oldSize = _size;
            const auto finalSize = _size + size;
            if (finalSize >= _capacity)
            {
                reserve(finalSize * _capacityMultiplier);
            }

            // TODO: use memxxx
            for (int64_t i = oldSize; i >= 0; --i)
            {
                _string[i + size] = _string[i];
            }

            _size += size;
            memcpy_s(_string, _size * sizeof(CharT), str, size * sizeof(CharT));
            _string[_size] = 0;
        }

        void popBack()
        {
            if (_size > 0)
            {
                tryToMakeAsDynamic();
                _string[--_size] = 0;
            }
        }

        void popFront()
        {
            if (_size > 0)
            {
                tryToMakeAsDynamic();
                for (std::size_t i = 1; i < _size; ++i)
                {
                    _string[i - 1] = _string[i];
                }
                _string[--_size] = 0;
            }
        }

        void copyTo(CharT* dest, std::size_t count, std::size_t offset = 0) const
        {
            if (!isEmpty())
            {
                memcpy_s(dest, count * sizeof(CharT), _string + offset, (std::min)(_size - offset, count) * sizeof(CharT));
                dest[count] = 0;
            }
        }

        void shrinkToFit()
        {
            if (isEmpty())
            {
                return;
            }
#if defined(UTILS_DEBUG)
            if constexpr (sizeof(CharT) == 1)
            {
                if (_string)
                {
                    StringTracer::instance().addChangedPolicyToDynamic(std::string(_string));
                }
            }
#endif

            const auto newCapacity = _size + 1;

            if (auto newString = new CharT[newCapacity])
            {
                memcpy_s(newString, newCapacity * sizeof(CharT), _string, newCapacity * sizeof(CharT));

                if (isDynamic())
                {
                    delete[] _string;
                }

                _capacity = newCapacity;
                _string = newString;
            }
        }

        [[nodiscard]] std::size_t capacity() const noexcept { return isStatic() ? _size + 1 : _capacity; }

        void insert(Iterator iterator, const CharT* str, std::size_t size = invalidSize)
        {
            DEBUG_ASSERT(iterator._owner == this);
            if (iterator._owner == this)
            {
                return insert(iterator._data - _string, str, size);
            }
        }

        void insert(int64_t pos, const CharT* str, std::size_t size = invalidSize)
        {
            if (size == invalidSize)
            {
                size = Toolset::Length(str);
            }

            tryToMakeAsDynamic();

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

            for (std::size_t i = pos; i < pos + size; ++i, ++str)
            {
                _string[i] = *str;
            }
            _string[finalSize] = 0;
            _size += size;
        }

        [[nodiscard]] bool isStatic() const noexcept { return _string && _capacity == _maxCapacity; }
        [[nodiscard]] bool isDynamic() const noexcept { return _string && _capacity != _maxCapacity; }

        [[nodiscard]] Comparison compare(StdStringViewT other, const bool isIgnoreCase = false) const
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
                for (std::size_t index = 0; index < other.size() && _string[index]; ++index)
                {
                    if (_string[index + 1] == 0 && other.size() == index + 1)
                    {
                        return Comparison::Equal;
                    }

                    const auto diff = Toolset::ToUpper(_string[index]) - Toolset::ToUpper(other[index]);
                    if (diff > 0 || _string[index + 1] == 0)
                    {
                        return Comparison::Greater;
                    }
                    if (diff < 0 || other.size() == index + 1)
                    {
                        return Comparison::Less;
                    }
                }
                return Comparison::None;
            }

            return Toolset::Cmp(_string, other.data());
        }

        [[nodiscard]] const CharT* find(StdStringViewT other, std::size_t baseOffset = 0) const noexcept
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
        [[nodiscard]] const CharT* findIgnoreCase(StdStringViewT other, std::size_t baseOffset = 0) const noexcept
        {
            if (isEmpty())
            {
                return nullptr;
            }

            if (other.empty())
            {
                return _string;
            }
            return Toolset::StrIStr(_string + baseOffset, other.data());
        }

        [[nodiscard]] const CharT* reverseFind(StdStringViewT other, std::size_t baseOffset = 0, std::size_t limitOffset = 0) const noexcept
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
        [[nodiscard]] std::vector<const CharT*> findAllIgnoreCase(StdStringViewT other) const noexcept
        {
            if (isEmpty() || other.empty())
            {
                return {};
            }

            std::vector<const CharT*> strings;

            CharT* foundStr = _string;
            do
            {
                if ((foundStr = Toolset::StrIStr(foundStr, other.data())))
                {
                    strings.push_back(foundStr);
                    ++foundStr;
                }
            } while (foundStr);

            return strings;
        }

        BaseString() { reserve(minAllocationSize); }

        template<class IterT>
        BaseString(IterT first, IterT last)
        {
            if (first != last)
            {
                // 32 is the minimum size to optimize working with small strings
                reserve(32);
                for (; first != last; ++first)
                {
                    pushBack(static_cast<CharT>(0));
                    std::char_traits<CharT>::assign(_string[_size - 1], *first);
                }
            }
        }

        BaseString(const CharT* str, std::size_t size = invalidSize)
        {
            if (str)
            {
                resize(size == invalidSize ? Toolset::Length(str) : size);
                memcpy_s(_string, _size * sizeof(CharT), str, _size * sizeof(CharT));
                _string[_size] = 0;
            }
        }

        BaseString(const char* str, std::size_t size = invalidSize)
            requires(sizeof(CharT) > 1)
        {
            if (str)
            {
                resize(size == invalidSize ? StringToolset<char>::Length(str) : size);
                for (std::size_t i = 0; i < _size; ++i)
                {
                    _string[i] = static_cast<CharT>(str[i]);
                }
            }
        }

        explicit BaseString(StdStringViewT str)
            : BaseString(str.data(), str.size())
        {
        }

        BaseString(const Self& other) { *this = other; }

        explicit BaseString(std::size_t reserveCount) { reserve(reserveCount); }

        Self& operator=(const CharT* str)
        {
            clear();
            const auto strSize = Toolset::Length(str);
            resize(strSize);
            memcpy_s(_string, _size * sizeof(CharT), str, strSize * sizeof(CharT));
            _string[_size] = 0;
            return *this;
        }

        Self& operator=(StdStringViewT other)
        {
            clear();
            resize(other.size());
            memcpy_s(_string, _size * sizeof(CharT), other.data(), other.size() * sizeof(CharT));
            _string[_size] = 0;
            return *this;
        }

        Self& operator=(const Self& other)
        {
            if (this == &other)
            {
                return *this;
            }

            clear();

            if (other.isDynamic())
            {
                resize(other._size);
                memcpy_s(_string, _size * sizeof(CharT), other._string, other._size * sizeof(CharT));
                _string[_size] = 0;
            }
            else
            {
                _string = other._string;
                _size = other._size;
                _capacity = other._capacity;
            }

            return *this;
        }

        BaseString(Self&& other) noexcept { *this = std::move(other); }

        Self& operator=(Self&& other) noexcept
        {
            clear();

            if (other.isDynamic())
            {
                _string = other._string;
                _size = other._size;
                _capacity = other._capacity;
#if defined(UTILS_DEBUG)
                if constexpr (sizeof(CharT) == 1)
                {
                    StringTracer::instance().addChangedPolicyToDynamic(std::string(_string));
                }
#endif
            }
            else
            {
                _string = other._string;
                _size = other._size;
                _capacity = other._capacity;
            }

            other._size = 0;
            other._string = nullptr;
            other._capacity = 0;

            return *this;
        }

        void clear()
        {
            if (_string)
            {
                if (isStatic())
                {
                    _string = nullptr;
                    _size = 0;
                    _capacity = 0;
                }
                else
                {
                    delete[] _string;
                    _string = nullptr;
                    _size = 0;
                    _capacity = 0;
                }
            }
        }

        void reserve(std::size_t newCapacity)
        {
#if defined(UTILS_DEBUG)
            if constexpr (sizeof(CharT) == 1)
            {
                if (isStatic() && _string)
                {
                    StringTracer::instance().addChangedPolicyToDynamic(std::string(_string));
                }
            }
#endif

            const auto oldCapacity = _capacity;

            if (newCapacity < minAllocationSize)
            {
                newCapacity = minAllocationSize;
            }

            if (auto* newString = new CharT[newCapacity])
            {
                newString[0] = 0;
                if (_string)
                {
                    memcpy(newString, _string, (std::min)(newCapacity, oldCapacity) * sizeof(CharT));
                }

                if (isDynamic())
                {
                    delete[] _string;
                }

                _string = newString;
                _capacity = newCapacity;
                if (oldCapacity != _maxCapacity && newCapacity < oldCapacity)
                {
                    _size = newCapacity - 1;
                }
                _string[_size] = 0;
            }
        }

        void resize(const std::size_t newSize)
        {
            if (newSize >= _capacity)
            {
                reserve(std::max(newSize * _capacityMultiplier, minAllocationSize));
            }

            _size = newSize;
            _string[newSize] = 0;
        }

        [[nodiscard]] BaseString<char> toASCII() const
        {
            BaseString<char> temp;
            temp.resize(_size);

            for (std::size_t i = 0; i < _size; ++i)
            {
                temp[i] = static_cast<char>(_string[i]);
            }
            _string[_size] = 0;
            return temp;
        }

        ~BaseString() { clear(); }

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
                    DEBUG_ASSERT(false);
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
                    DEBUG_ASSERT(false);
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

        static std::size_t GetLinesCountInText(const CharT* source, const CharT* end = nullptr, LineSeparator separator = LineSeparator::LF) noexcept
        {
            if (!DEBUG_ASSERT_VAL(source, "Impossible to calculate count of lines in the text, because was passed NULL pointer to the string."))
            {
                return 0;
            }

            if (end == nullptr)
            {
                end = source + Toolset::Length(source);
            }

            std::size_t count = 0;
            const auto* string = source;
            while ((string = FindNextLine(string, end, separator)))
            {
                ++count;
            }

            return ++count;
        }

        static const CharT* FindNextLine(const CharT* string, const CharT* end = nullptr, LineSeparator separator = LineSeparator::LF)
        {
            const auto* result = StringToolset<CharT>::StrStr(string, GetLineSeparatorString(separator));
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

            const auto* result = StringToolset<CharT>::ReverseStrStr(begin, GetLineSeparatorString(separator), end);
            if (result != nullptr && result != begin)
            {
                result += GetLineSeparatorStringSize(separator);
            }

            return result;
        }

        /**
         * @brief Can take functions of next types:
         * bool(String) - this function will work until it gets 'false' in return
         * void(String) - will iterate without stopping through all main string
         */
        template<class FuncT>
        void forEachByLine(FuncT&& callback, LineSeparator separator = LineSeparator::LF)
        {
            ForEachByLineImpl<false, FuncT>(this, std::forward<decltype(callback)>(callback), separator);
        }

        /**
         * @brief Can take functions of next types:
         * bool(String) - this function will work until it gets 'false' in return
         * void(String) - will iterate without stopping through all main string
         */
        template<class FuncT>
        void forEachByLine(FuncT&& callback, LineSeparator separator = LineSeparator::LF) const
        {
            ForEachByLineImpl<true, FuncT>(this, std::forward<decltype(callback)>(callback), separator);
        }

        explicit BaseString(StringDataReadOnlyT data)
            : _string{ data.str },
              _size{ data.size },
              _capacity{ _maxCapacity }
        {
        }

    protected:
        void markAsStatic() noexcept
        {
            if (isEmpty())
            {
                return;
            }

#if defined(UTILS_DEBUG)
            DEBUG_ASSERT(Core::StringPool<char>::instance().isStatic(_string));
#endif
        }

        void tryToMakeAsDynamic()
        {
            if (isStatic() && !isEmpty())
            {
                reserve(_size);
            }
        }

    protected:
        CharT* _string = nullptr;
        std::size_t _size = 0;
        std::size_t _capacity = 0;

        static constexpr std::size_t _maxCapacity = std::numeric_limits<std::size_t>::max();
        static constexpr std::size_t _capacityMultiplier = 2;

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
                std::size_t strSize = invalidSize;
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

        template<class T>
        Self& operator<<(const T& value)
        {
            FormatFirst(value);
            return *this;
        }

    protected:
        template<class T>
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
#if defined(UTILS_DEBUG)
            if (!this->find(static_cast<const CharT*>(expr)) && this->_string)
            {
                std::stringstream ss;
                ss << "Can't find replace token '" << static_cast<const CharT*>(expr) << "' while formatting of string: " << this->_string << '\n';
                DEBUG_ASSERT(ss.str().c_str());
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

inline Core::BaseString<char> operator""_atom(const char* str, std::size_t size)
{
    static auto& pool = Core::StringPool<char>::instance();
    return Core::BaseString{ pool.intern(str, size) };
}

inline Core::BaseString<wchar_t> operator""_atom(const wchar_t* str, std::size_t size)
{
    static auto& pool = Core::StringPool<wchar_t>::instance();
    return Core::BaseString{ pool.intern(str, size) };
}

inline Core::BaseString<char> operator""_dyn(const char* str, std::size_t size)
{
    return { str, size };
}

inline Core::BaseString<wchar_t> operator""_dyn(const wchar_t* str, std::size_t size)
{
    return { str, size };
}

template<class CharType>
[[nodiscard]] bool operator>(typename Core::StringToolset<CharType>::StdStringViewT str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1);
}

template<class CharType>
[[nodiscard]] bool operator>=(typename Core::StringToolset<CharType>::StdStringViewT str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1) || (str1 == str2);
}

template<class CharType>
[[nodiscard]] bool operator<(typename Core::StringToolset<CharType>::StdStringViewT str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1);
}

template<class CharType>
[[nodiscard]] bool operator<=(typename Core::StringToolset<CharType>::StdStringViewT str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1) || (str1 == str2);
}

template<class CharType>
[[nodiscard]] Core::BaseString<CharType> operator+(typename Core::StringToolset<CharType>::StdStringViewT str1,
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

inline Core::StringFormatter<char> operator""_f(const char* str, std::size_t size)
{
    return { str, size };
}

inline Core::StringFormatter<wchar_t> operator""_f(const wchar_t* str, std::size_t size)
{
    return { str, size };
}

std::ostream& operator<<(std::ostream& stream, const Core::StringAtom& s);
std::istream& operator>>(std::istream& stream, Core::StringAtom& s);
