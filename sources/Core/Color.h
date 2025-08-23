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

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Core
{

    struct Color3;
    struct Color4;
    struct NormColor3;
    struct NormColor4;

    /**
     * @brief Non-normalized Color. Can get values 0 - 255
     */
    struct Color4 : public glm::vec<4, uint8_t>
    {
        using Parent = glm::vec<4, uint8_t>;
        using Parent::vec;

        constexpr explicit Color4(const Parent& value)
            : Parent(value)
        {
        }

        [[nodiscard]] static Color4 From(const NormColor4& other) noexcept;
        void from(const NormColor4& other) noexcept;
    };

    /**
     * @brief Non-normalized Color. Can get values 0 - 255
     */
    struct Color3 : public glm::vec<3, uint8_t>
    {
        using Parent = glm::vec<3, uint8_t>;
        using Parent::vec;

        constexpr explicit Color3(const Parent& value)
            : Parent(value)
        {
        }

        [[nodiscard]] static Color3 From(const NormColor3& other) noexcept;
        void from(const NormColor3& other) noexcept;
    };

    /**
     * @brief Normalized Color. Can get values 0.0 - 1.0
     */
    struct NormColor4 : public glm::vec<4, float>
    {
        using Parent = glm::vec<4, float>;
        using Parent::vec;

        constexpr explicit NormColor4(const Parent& value)
            : Parent(value)
        {
        }

        [[nodiscard]] static NormColor4 From(const Color4& other) noexcept;
        void from(const Color4& other) noexcept;
    };

    /**
     * @brief Normalized Color. Can get values 0.0 - 1.0
     */
    struct NormColor3 : public glm::vec<3, float>
    {
        using Parent = glm::vec<3, float>;
        using Parent::vec;

        constexpr explicit NormColor3(const Parent& value)
            : Parent(value)
        {
        }

        [[nodiscard]] static NormColor3 From(const Color3& other) noexcept;
        void from(const Color3& other) noexcept;
    };

} // namespace Core