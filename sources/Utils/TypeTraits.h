// MIT License
//
// Copyright (c) 2018-2025 Valerii Koniushenko
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

#include <type_traits>

namespace Utils
{

    template<class From, class To>
    auto is_non_narrowing_convertible_helper_function(int) -> decltype(std::declval<void(To)>()({ std::declval<From>() }), std::true_type{});

    template<class From, class To>
    auto is_non_narrowing_convertible_helper_function(...) -> std::false_type;

    template<class From, class To,
             bool = (std::is_arithmetic_v<From> || std::is_enum_v<From> || std::is_pointer_v<From> || std::is_member_pointer_v<From>) &&
                    (std::is_arithmetic_v<To> || std::is_enum_v<To>)>
    struct is_non_narrowing_convertible_helper;

    template<class From, class To>
    struct is_non_narrowing_convertible_helper<From, To, true> : decltype(is_non_narrowing_convertible_helper_function<From, To>(0))
    {
    };

    template<class From, class To>
    struct is_non_narrowing_convertible_helper<From, To, false> : std::is_convertible<From, To>
    {
    };

    template<class From, class To>
    struct is_non_narrowing_convertible : is_non_narrowing_convertible_helper<From, To>
    {
    };

    template<class From, class To>
    inline constexpr bool is_non_narrowing_convertible_v = is_non_narrowing_convertible<From, To>::value;

    template<class From, class To>
    struct is_nothrow_non_narrowing_convertible : std::conjunction<is_non_narrowing_convertible<From, To>, std::is_nothrow_convertible<From, To>>
    {
    };

    template<class From, class To>
    inline constexpr bool is_nothrow_non_narrowing_convertible_v = is_nothrow_non_narrowing_convertible<From, To>::value;

    template<class T>
    struct is_string_literal : std::false_type
    {
    };

    template<class CharT, size_t N>
    struct is_string_literal<CharT (&)[N]> : std::true_type
    {
    };

    template<class CharT, class T>
    inline constexpr bool is_string_literal_v = is_string_literal<T>::value;

} // namespace Utils