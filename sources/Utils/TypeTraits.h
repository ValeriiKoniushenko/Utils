// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

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