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

        [[nodiscard]] constexpr static Color4 From(const NormColor4& other) noexcept;
        constexpr void from(const NormColor4& other) noexcept;
        [[nodiscard]] constexpr NormColor4 toNorm() const noexcept;
        [[nodiscard]] constexpr Parent toGlm() const noexcept { return Parent{ r, g, b, a }; }
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

        [[nodiscard]] constexpr static Color3 From(const NormColor3& other) noexcept;
        [[nodiscard]] constexpr NormColor3 toNorm() const noexcept;
        constexpr void from(const NormColor3& other) noexcept;
        [[nodiscard]] constexpr Parent toGlm() const noexcept { return Parent{ r, g, b }; }
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

        [[nodiscard]] constexpr static NormColor4 From(const Color4& other) noexcept;
        [[nodiscard]] constexpr Color4 toColor() const noexcept;
        constexpr void from(const Color4& other) noexcept;
        [[nodiscard]] constexpr Parent toGlm() const noexcept { return Parent{ r, g, b, a }; }
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

        [[nodiscard]] constexpr static NormColor3 From(const Color3& other) noexcept;
        [[nodiscard]] constexpr Color3 toColor() const noexcept;
        constexpr void from(const Color3& other) noexcept;
        [[nodiscard]] constexpr Parent toGlm() const noexcept { return Parent{ r, g, b }; }
    };

    constexpr void Color4::from(const NormColor4& other) noexcept
    {
        *this = Color4::From(other);
    }

    constexpr NormColor4 Color4::toNorm() const noexcept
    {
        return NormColor4::From(*this);
    }

    constexpr Color4 Color4::From(const NormColor4& other) noexcept
    {
        Color4 out;
        out.r = static_cast<uint8_t>(other.r * 255.f);
        out.g = static_cast<uint8_t>(other.g * 255.f);
        out.b = static_cast<uint8_t>(other.b * 255.f);
        out.a = static_cast<uint8_t>(other.a * 255.f);
        return out;
    }

    constexpr void Color3::from(const NormColor3& other) noexcept
    {
        *this = Color3::From(other);
    }

    constexpr Color3 Color3::From(const NormColor3& other) noexcept
    {
        Color3 out;
        out.r = static_cast<uint8_t>(other.r * 255.f);
        out.g = static_cast<uint8_t>(other.g * 255.f);
        out.b = static_cast<uint8_t>(other.b * 255.f);
        return out;
    }

    constexpr NormColor3 Color3::toNorm() const noexcept
    {
        return NormColor3::From(*this);
    }

    constexpr NormColor4 NormColor4::From(const Color4& other) noexcept
    {
        return { static_cast<float>(other.r) / 255.f, static_cast<float>(other.g) / 255.f, static_cast<float>(other.b) / 255.f,
                 static_cast<float>(other.a) / 255.f };
    }

    constexpr Color4 NormColor4::toColor() const noexcept
    {
        return Color4::From(*this);
    }

    constexpr void NormColor4::from(const Color4& other) noexcept
    {
        *this = NormColor4::From(other);
    }

    constexpr NormColor3 NormColor3::From(const Color3& other) noexcept
    {
        return {
            static_cast<float>(other.r) / 255.f,
            static_cast<float>(other.g) / 255.f,
            static_cast<float>(other.b) / 255.f,
        };
    }

    constexpr Color3 NormColor3::toColor() const noexcept
    {
        return Color3::From(*this);
    }

    constexpr void NormColor3::from(const Color3& other) noexcept
    {
        *this = NormColor3::From(other);
    }

    using RGB = Color3;
    using RGBA = Color4;
    using RGBn = NormColor3;
    using RGBAn = NormColor4;

} // namespace Core