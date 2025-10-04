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

#include "Color.h"

#include <algorithm>

using namespace Core;

std::ostream& operator<<(std::ostream& os, const Color3& color)
{
    return os << static_cast<int>(color.x) << " " << static_cast<int>(color.y) << " " << static_cast<int>(color.z);
}

std::ostream& operator<<(std::ostream& os, const Color4& color)
{
    return os << static_cast<int>(color.x) << " " << static_cast<int>(color.y) << " " << (color.z) << " " << static_cast<int>(color.w);
}

std::ostream& operator<<(std::ostream& os, const NormColor3& normColor)
{
    return os << normColor.toColor();
}

std::ostream& operator<<(std::ostream& os, const NormColor4& normColor)
{
    return os << normColor.toColor();
}

std::istream& operator>>(std::istream& is, Color3& color)
{
    int r = 0, g = 0, b = 0;
    is >> r >> g >> b;
    color.x = static_cast<decltype(color.x)>(std::clamp(r, 0, 255));
    color.y = static_cast<decltype(color.y)>(std::clamp(g, 0, 255));
    color.z = static_cast<decltype(color.z)>(std::clamp(b, 0, 255));

    return is;
}

std::istream& operator>>(std::istream& is, Color4& color)
{
    int r = 0, g = 0, b = 0, a = 0;
    is >> r >> g >> b >> a;
    color.x = static_cast<decltype(color.x)>(std::clamp(r, 0, 255));
    color.y = static_cast<decltype(color.y)>(std::clamp(g, 0, 255));
    color.z = static_cast<decltype(color.z)>(std::clamp(b, 0, 255));
    color.w = static_cast<decltype(color.w)>(std::clamp(a, 0, 255));

    return is;
}

std::istream& operator>>(std::istream& is, NormColor3& normColor)
{
    Color3 tmp;
    is >> tmp;
    normColor = NormColor3::From(tmp);
    return is;
}

std::istream& operator>>(std::istream& is, NormColor4& normColor)
{
    Color4 tmp;
    is >> tmp;
    normColor = NormColor4::From(tmp);
    return is;
}
