// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "Color.h"

#include <algorithm>
#include <iostream>

using namespace Core;

std::ostream& operator<<(std::ostream& os, const Color3& color)
{
    return os << static_cast<int>(color.x) << " " << static_cast<int>(color.y) << " "
              << static_cast<int>(color.z);
}

std::ostream& operator<<(std::ostream& os, const Color4& color)
{
    return os << static_cast<int>(color.x) << " " << static_cast<int>(color.y) << " "
              << static_cast<int>(color.z) << " " << static_cast<int>(color.w);
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
    int r = 0;
    int g = 0;
    int b = 0;
    is >> r >> g >> b;
    color.x = static_cast<decltype(color.x)>(std::clamp(r, 0, 255));
    color.y = static_cast<decltype(color.y)>(std::clamp(g, 0, 255));
    color.z = static_cast<decltype(color.z)>(std::clamp(b, 0, 255));

    return is;
}

std::istream& operator>>(std::istream& is, Color4& color)
{
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;
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
