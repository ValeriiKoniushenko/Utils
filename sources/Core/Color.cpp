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

#include "Color.h"

namespace Core
{

    void Color4::from(const NormColor4& other) noexcept
    {
        *this = Color4::From(other);
    }

    Color4 Color4::From(const NormColor4& other) noexcept
    {
        Color4 out;
        out.r = static_cast<uint8_t>(other.r * 255.f);
        out.g = static_cast<uint8_t>(other.g * 255.f);
        out.b = static_cast<uint8_t>(other.b * 255.f);
        out.a = static_cast<uint8_t>(other.a * 255.f);
        return out;
    }

    void Color3::from(const NormColor3& other) noexcept
    {
        *this = Color3::From(other);
    }

    Color3 Color3::From(const NormColor3& other) noexcept
    {
        Color3 out;
        out.r = static_cast<uint8_t>(other.r * 255.f);
        out.g = static_cast<uint8_t>(other.g * 255.f);
        out.b = static_cast<uint8_t>(other.b * 255.f);
        return out;
    }

    NormColor4 NormColor4::From(const Color4& other) noexcept
    {
        NormColor4 out;
        out.r = static_cast<float>(other.r) / 255.f;
        out.g = static_cast<float>(other.g) / 255.f;
        out.b = static_cast<float>(other.b) / 255.f;
        out.a = static_cast<float>(other.a) / 255.f;
        return out;
    }

    void NormColor4::from(const Color4& other) noexcept
    {
        *this = NormColor4::From(other);
    }

    NormColor3 NormColor3::From(const Color3& other) noexcept
    {
        NormColor3 out;
        out.r = static_cast<float>(other.r) / 255.f;
        out.g = static_cast<float>(other.g) / 255.f;
        out.b = static_cast<float>(other.b) / 255.f;
        return out;
    }

    void NormColor3::from(const Color3& other) noexcept
    {
        *this = NormColor3::From(other);
    }

} // namespace Core