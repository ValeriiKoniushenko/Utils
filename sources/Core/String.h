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

        [[nodiscard]] StringDataReadOnly<CharType> ToReadOnly() noexcept
        {
            return StringDataReadOnly<CharType>{ str.get(), size };
        }

        SmartPointer str;
        typename Settings::SizeT size = Settings::invalidSize;
        std::size_t copyCounts = 0;
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
        class Iterator : public RandomAccessIterator<CharT>
        {
        public:
            using Self = Iterator;
            using Super = RandomAccessIterator<CharT>;

        public:
            Iterator() = default;

        protected:
            explicit Iterator(CharT* data)
                : Iterator{ data }
            {
            }

            void MakeStep(int step) noexcept override { this->_data += step; }

            [[nodiscard]] Comparison operator<=>(const Super& other) const noexcept override
            {
                if (!this->_data || !other._data)
                {
                    return Comparison::None;
                }

                if (*this->_data > *other._data)
                {
                    return Comparison::Greater;
                }

                if (*this->_data == *other._data)
                {
                    return Comparison::Equal;
                }

                if (*this->_data < *other._data)
                {
                    return Comparison::Less;
                }

                return Comparison::None;
            }

            friend class BaseString<CharT>;
        };

    public:
        [[nodiscard]] static Self Intern(const CharT* newString, SizeT size = Settings::invalidSize)
        {
            return Self{ StringPool::Instance().Add(newString, size == Settings::invalidSize ? Toolset::Length(newString) : size) };
        }

    private:
        constexpr explicit BaseString(StringDataReadOnlyT data)
            : _string{ data.str },
              _size{ data.size }
        {
        }


        /*[[nodiscard]] constexpr const CharT* CStr() const noexcept { return GetData().string; }
        [[nodiscard]] constexpr SizeT Size() const noexcept { return GetData().size; }
        [[nodiscard]] constexpr SizeT Length() const noexcept { return GetData().size == 0; }
        [[nodiscard]] constexpr bool IsEmpty() const noexcept { return GetData().size == 0; }
        [[nodiscard]] constexpr explicit operator const CharT*() const noexcept { return GetData().string; }
        [[nodiscard]] constexpr CharT operator[](std::size_t index) const { return GetData().string[index]; }

        [[nodiscard]] constexpr bool operator==(const Self& other) const { return GetData().string == other.GetData().string; }
        [[nodiscard]] constexpr bool operator!=(const Self& other) const { return GetData().string != other.GetData().string; }
        [[nodiscard]] bool operator>(const Self& other) const
        {
            return Toolset::Cmp(GetData().string, other.GetData().string) == Comparison::Greater;
        }
        [[nodiscard]] bool operator>=(const Self& other) const
        {
            const auto result = Toolset::Cmp(GetData().string, other.GetData().string);
            return result == Comparison::Greater || result == Comparison::Equal;
        }
        [[nodiscard]] bool operator<(const Self& other) const { return Toolset::Cmp(GetData().string, other.GetData().string) == Comparison::Less; }
        [[nodiscard]] bool operator<=(const Self& other) const
        {
            const auto result = Toolset::Cmp(GetData().string, other.GetData().string);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const CharT* other) const { return Toolset::Cmp(GetData().string, other) == Comparison::Equal; }
        [[nodiscard]] bool operator!=(const CharT* other) const { return Toolset::Cmp(GetData().string, other) != Comparison::Equal; }
        [[nodiscard]] bool operator>(const CharT* other) const { return Toolset::Cmp(GetData().string, other) == Comparison::Greater; }
        [[nodiscard]] bool operator>=(const CharT* other) const
        {
            const auto result = Toolset::Cmp(GetData().string, other);
            return result == Comparison::Greater || result == Comparison::Equal;
        }
        [[nodiscard]] bool operator<(const CharT* other) const { return Toolset::Cmp(GetData().string, other) == Comparison::Less; }
        [[nodiscard]] bool operator<=(const CharT* other) const
        {
            const auto result = Toolset::Cmp(GetData().string, other);
            return result == Comparison::Less || result == Comparison::Equal;
        }

        [[nodiscard]] bool operator==(const StringT& other) const { return *this == other.data(); }
        [[nodiscard]] bool operator>(const StringT& other) const { return *this > other.data(); }
        [[nodiscard]] bool operator>=(const StringT& other) const { return *this >= other.data(); }
        [[nodiscard]] bool operator<(const StringT& other) const { return *this < other.data(); }
        [[nodiscard]] bool operator<=(const StringT& other) const { return *this <= other.data(); }

        [[nodiscard]] constexpr bool operator!() const noexcept { return !GetData().string; }
        [[nodiscard]] constexpr operator bool() const noexcept { return GetData().string; }

        [[nodiscard]] constexpr CharT Front() const
        {
            Data data = GetData();
            if (!data.string)
            {
                Assert("Was get a null string");
                return {};
            }

            return data.string[0];
        }

        [[nodiscard]] constexpr CharT Back() const
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

        [[nodiscard]] std::vector<StringT> Split(const StringT& delimiter) const
        {
            StringT string;
            {
                const Data data = GetData();
                if (!data.string)
                {
                    Assert("Was get a null string");
                    return {};
                }
                string = data.string;
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
            return Toolset::ToInt(GetData().string);
        }

        template<>
        [[nodiscard]] float ConvertTo() const noexcept
        {
            return Toolset::ToFloat(GetData().string);
        }

        template<>
        [[nodiscard]] long long ConvertTo() const noexcept
        {
            return Toolset::ToLongLong(GetData().string);
        }

        [[nodiscard]] static std::size_t operator()(const Self& string) noexcept { return string.MakeHash(); }

        [[nodiscard]] std::size_t MakeHash() const noexcept
        {
            const auto& data = GetData();
            return std::hash<std::string_view>{}({ data.string, data.size });
        }

        [[nodiscard]] IteratorT begin() noexcept { return IteratorT{ GetNonConstData().string }; }
        [[nodiscard]] ConstIteratorT begin() const noexcept { return ConstIteratorT{ GetNonConstData().string }; }
        [[nodiscard]] ConstIteratorT cbegin() const noexcept { return ConstIteratorT{ GetNonConstData().string }; }

        [[nodiscard]] IteratorT end() noexcept
        {
            auto data = GetNonConstData();
            return IteratorT{ data.string + data.size };
        }
        [[nodiscard]] ConstIteratorT end() const noexcept
        {
            auto data = GetNonConstData();
            return ConstIteratorT{ data.string + data.size };
        }
        [[nodiscard]] ConstIteratorT cend() const noexcept
        {
            auto data = GetNonConstData();
            return ConstIteratorT{ data.string + data.size };
        }

        Self& TrimStart(StringViewT value) noexcept
        {
            if (value)
            {

                _isDirty = true;
            }
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
        */
    protected:
        CharT* _string = nullptr;
        SizeT _size = 0;
        StringPolicy _policy = StringPolicy::Static;
    };

    using StringAtom = BaseString<char>;

} // namespace Core

namespace std
{

    /*template<class CharType>
    struct hash<Core::BaseString<CharType>>
    {
        size_t operator()(const Core::BaseString<CharType>& x) const noexcept { return x.MakeHash(); }
    };*/

} // namespace std

constexpr Core::BaseString<char> operator""_atom(const char* str, std::size_t size) noexcept
{
    return Core::BaseString<char>::Intern(str, size);
}

/*template<class CharType>
[[nodiscard]] bool operator>(const CharType* str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator>=(const CharType* str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1) || (str1 == str2);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator<(const CharType* str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator<=(const CharType* str1, const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1) || (str1 == str2);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator>(const std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>& str1,
                             const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator>=(const std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>& str1,
                              const Core::BaseString<CharType>& str2)
{
    return !(str2 > str1) || (str1 == str2);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator<(const std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>& str1,
                             const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1);
}

template<class CharType, Core::StringPolicy StringPolicy>
[[nodiscard]] bool operator<=(const std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>& str1,
                              const Core::BaseString<CharType>& str2)
{
    return !(str2 < str1) || (str1 == str2);
}*/
