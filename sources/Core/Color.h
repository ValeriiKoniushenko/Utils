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

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <iostream>

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

        constexpr explicit Color4(const Color3& value, uint8_t alpha = 255);

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

    constexpr Color4::Color4(const Color3& value, uint8_t alpha)
        : Parent(value.r, value.g, value.b, alpha)
    {
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

    // Default colors
    static constinit auto Color3_Red = Color3(255, 0, 0);
    static constinit auto Color3_Green = Color3(0, 255, 0);
    static constinit auto Color3_Blue = Color3(0, 0, 255);
    static constinit auto Color3_Yellow = Color3(255, 255, 0);
    static constinit auto Color3_Cyan = Color3(0, 255, 255);
    static constinit auto Color3_Magenta = Color3(255, 0, 255);
    static constinit auto Color3_Orange = Color3(255, 165, 0);
    static constinit auto Color3_Purple = Color3(128, 0, 128);
    static constinit auto Color3_Gray = Color3(128, 128, 128);
    static constinit auto Color3_White = Color3(255, 255, 255);
    static constinit auto Color3_Black = Color3(0, 0, 0);
    static constinit auto Color3_LightGray = Color3(211, 211, 211);
    static constinit auto Color3_DarkGray = Color3(64, 64, 64);
    static constinit auto Color3_Brown = Color3(139, 69, 19);
    static constinit auto Color3_Tan = Color3(210, 180, 140);
    static constinit auto Color3_Olive = Color3(128, 128, 0);
    static constinit auto Color3_Lime = Color3(50, 205, 50);
    static constinit auto Color3_SeaGreen = Color3(46, 139, 87);
    static constinit auto Color3_Teal = Color3(0, 128, 128);
    static constinit auto Color3_Turquoise = Color3(64, 224, 208);
    static constinit auto Color3_SkyBlue = Color3(135, 206, 235);
    static constinit auto Color3_DeepSkyBlue = Color3(0, 191, 255);
    static constinit auto Color3_Navy = Color3(0, 0, 128);
    static constinit auto Color3_Indigo = Color3(75, 0, 130);
    static constinit auto Color3_Violet = Color3(238, 130, 238);
    static constinit auto Color3_Pink = Color3(255, 192, 203);
    static constinit auto Color3_HotPink = Color3(255, 105, 180);
    static constinit auto Color3_Coral = Color3(255, 127, 80);
    static constinit auto Color3_Gold = Color3(255, 215, 0);
    static constinit auto Color3_Silver = Color3(192, 192, 192);

    static constinit auto Color4_Red = Color4(255, 0, 0, 255);
    static constinit auto Color4_Green = Color4(0, 255, 0, 255);
    static constinit auto Color4_Blue = Color4(0, 0, 255, 255);
    static constinit auto Color4_Yellow = Color4(255, 255, 0, 255);
    static constinit auto Color4_Cyan = Color4(0, 255, 255, 255);
    static constinit auto Color4_Magenta = Color4(255, 0, 255, 255);
    static constinit auto Color4_Orange = Color4(255, 165, 0, 255);
    static constinit auto Color4_Purple = Color4(128, 0, 128, 255);
    static constinit auto Color4_Gray = Color4(128, 128, 128, 255);
    static constinit auto Color4_White = Color4(255, 255, 255, 255);
    static constinit auto Color4_Black = Color4(0, 0, 0, 255);
    static constinit auto Color4_LightGray = Color4(211, 211, 211, 255);
    static constinit auto Color4_DarkGray = Color4(64, 64, 64, 255);
    static constinit auto Color4_Brown = Color4(139, 69, 19, 255);
    static constinit auto Color4_Tan = Color4(210, 180, 140, 255);
    static constinit auto Color4_Olive = Color4(128, 128, 0, 255);
    static constinit auto Color4_Lime = Color4(50, 205, 50, 255);
    static constinit auto Color4_SeaGreen = Color4(46, 139, 87, 255);
    static constinit auto Color4_Teal = Color4(0, 128, 128, 255);
    static constinit auto Color4_Turquoise = Color4(64, 224, 208, 255);
    static constinit auto Color4_SkyBlue = Color4(135, 206, 235, 255);
    static constinit auto Color4_DeepSkyBlue = Color4(0, 191, 255, 255);
    static constinit auto Color4_Navy = Color4(0, 0, 128, 255);
    static constinit auto Color4_Indigo = Color4(75, 0, 130, 255);
    static constinit auto Color4_Violet = Color4(238, 130, 238, 255);
    static constinit auto Color4_Pink = Color4(255, 192, 203, 255);
    static constinit auto Color4_HotPink = Color4(255, 105, 180, 255);
    static constinit auto Color4_Coral = Color4(255, 127, 80, 255);
    static constinit auto Color4_Gold = Color4(255, 215, 0, 255);
    static constinit auto Color4_Silver = Color4(192, 192, 192, 255);

} // namespace Core

std::ostream& operator<<(std::ostream& os, const Core::Color3& color);
std::ostream& operator<<(std::ostream& os, const Core::Color4& color);
std::ostream& operator<<(std::ostream& os, const Core::NormColor3& normColor);
std::ostream& operator<<(std::ostream& os, const Core::NormColor4& normColor);

std::istream& operator>>(std::istream& is, Core::Color3& color);
std::istream& operator>>(std::istream& is, Core::Color4& color);
std::istream& operator>>(std::istream& is, Core::NormColor3& normColor);
std::istream& operator>>(std::istream& is, Core::NormColor4& normColor);
