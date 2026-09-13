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

    // tag::fundamental_concepts[]
    template<class T>
    concept IsArithmetic = std::is_arithmetic_v<T>;

    template<class T>
    concept IsFloating = std::is_floating_point_v<T>;

    template<class T>
    concept IsIntegral = std::is_integral_v<T>;
    // end::fundamental_concepts[]

} // namespace Utils
