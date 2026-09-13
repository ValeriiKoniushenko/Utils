// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "Utils/Concepts.h"

#include <cmath>
#include <limits>

namespace Math
{

    // tag::math_helpers[]
    template<Utils::IsArithmetic T>
    [[nodiscard]] constexpr T Abs(T value) noexcept
    {
        return value < 0 ? -value : value;
    }

    template<Utils::IsFloating T, T Epsilon = std::numeric_limits<T>::epsilon()>
    [[nodiscard]] constexpr bool IsEqual(T n1, T n2) noexcept
    {
        return Abs(n1 - n2) < Epsilon;
    }

    template<Utils::IsFloating T, T Epsilon = std::numeric_limits<T>::epsilon()>
    [[nodiscard]] constexpr bool IsZero(T n) noexcept
    {
        return Abs(n) < Epsilon;
    }
    // end::math_helpers[]

} // namespace Math
