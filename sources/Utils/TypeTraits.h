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

#include <type_traits>

namespace Utils
{

    template<class From, class To>
    decltype(std::declval<void(To)>()({ std::declval<From>() }), std::true_type{})
        IsNonNarrowingConvertibleHelperFunction(int);

    template<class From, class To>
    std::false_type IsNonNarrowingConvertibleHelperFunction(...);

    template<class From, class To,
             bool = (std::is_arithmetic_v<From> || std::is_enum_v<From> || std::is_pointer_v<From>
                     || std::is_member_pointer_v<From>)
                    && (std::is_arithmetic_v<To> || std::is_enum_v<To>)>
    struct IsNonNarrowingConvertibleHelper;

    template<class From, class To>
    struct IsNonNarrowingConvertibleHelper<From, To, true> :
        decltype(IsNonNarrowingConvertibleHelperFunction<From, To>(0))
    {
    };

    template<class From, class To>
    struct IsNonNarrowingConvertibleHelper<From, To, false> : std::is_convertible<From, To>
    {
    };

    template<class From, class To>
    struct IsNonNarrowingConvertible : IsNonNarrowingConvertibleHelper<From, To>
    {
    };

    template<class From, class To>
    inline constexpr bool IsNonNarrowingConvertibleV = IsNonNarrowingConvertible<From, To>::value;

    template<class From, class To>
    struct IsNothrowNonNarrowingConvertible :
        std::conjunction<IsNonNarrowingConvertible<From, To>, std::is_nothrow_convertible<From, To>>
    {
    };

    template<class From, class To>
    inline constexpr bool IsNothrowNonNarrowingConvertible_v
        = IsNothrowNonNarrowingConvertible<From, To>::value;

    template<class T>
    struct IsStringLiteral : std::false_type
    {
    };

    template<class CharT, std::size_t N>
    struct IsStringLiteral<CharT (&)[N]> : std::true_type
    {
    };

    template<class CharT, class T>
    inline constexpr bool IsStringLiteralV = IsStringLiteral<T>::value;

    template<typename T>
    struct AlwaysFalse : std::false_type
    {
    };

    template<class T>
    inline constexpr bool AlwaysFalseV = AlwaysFalse<T>::value;

    template<typename, typename = void>
    struct HasValueType : std::false_type
    {
    };

    template<typename T>
    struct HasValueType<T, decltype(T::value_type, void())> : std::true_type
    {
    };

} // namespace Utils