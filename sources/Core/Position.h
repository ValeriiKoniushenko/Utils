// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
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

#include "glm/glm.hpp"

namespace Core
{
    template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
    struct GlobalPosition : public glm::vec<L, T, Q>
    {
        static_assert("Incomplete type. You can use only 2(XY) or 3(XYZ).");
    };

    template<typename T, glm::qualifier Q>
    struct GlobalPosition<2, T, Q> : public glm::vec<2, T, Q>
    {
        explicit constexpr GlobalPosition(T x = {}, T y = {}) : glm::vec2{ x, y } {}
    };

    template<typename T, glm::qualifier Q>
    struct GlobalPosition<3, T, Q> : public glm::vec<3, T, Q>
    {
        explicit constexpr GlobalPosition(T x = {}, T y = {}, T z = {}) : glm::vec3{ x, y, z } {}
    };

    using GlobalPosition3F = GlobalPosition<3, float>;
    using GlobalPosition2F = GlobalPosition<2, float>;

    using GlobalPosition3D = GlobalPosition<3, double>;
    using GlobalPosition2D = GlobalPosition<2, double>;

    using GlobalPosition3I = GlobalPosition<3, int>;
    using GlobalPosition2I = GlobalPosition<2, int>;

} // namespace Core
