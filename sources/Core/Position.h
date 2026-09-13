// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

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
        constexpr static int pcs = 2;
        using Parent = glm::vec<pcs, T, Q>;
        using glm::vec<pcs, T, Q>::vec;

        constexpr explicit GlobalPosition(Parent value)
            : Parent(value)
        {
        }
    };

    template<typename T, glm::qualifier Q>
    struct GlobalPosition<3, T, Q> : public glm::vec<3, T, Q>
    {
        constexpr static int pcs = 3;
        using Parent = glm::vec<pcs, T, Q>;
        using glm::vec<pcs, T, Q>::vec;

        constexpr explicit GlobalPosition(const Parent& value)
            : Parent(value)
        {
        }
    };

    using GlobalPosition3F = GlobalPosition<3, float>;
    using GlobalPosition2F = GlobalPosition<2, float>;

    using GlobalPosition3D = GlobalPosition<3, double>;
    using GlobalPosition2D = GlobalPosition<2, double>;

    using GlobalPosition3I = GlobalPosition<3, int>;
    using GlobalPosition2I = GlobalPosition<2, int>;

    template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
    struct RelativePosition : public glm::vec<L, T, Q>
    {
        static_assert("Incomplete type. You can use only 2(XY) or 3(XYZ).");
    };

    template<typename T, glm::qualifier Q>
    struct RelativePosition<2, T, Q> : public glm::vec<2, T, Q>
    {
        constexpr static int pcs = 2;
        using Parent = glm::vec<pcs, T, Q>;
        using glm::vec<pcs, T, Q>::vec;
        using Global = GlobalPosition<pcs, T, Q>;

        constexpr explicit RelativePosition(Parent value)
            : Parent(value)
        {
        }

        [[nodiscard]] constexpr Global castToGlobal() const { return Global{ this->x, this->y }; }

        [[nodiscard]] constexpr Global toGlobal(const Global& origin) const
        {
            return Global{ origin + castToGlobal() };
        }
        [[nodiscard]] constexpr RelativePosition toGlobal(const RelativePosition& origin) const
        {
            return RelativePosition{ origin + *this };
        }
    };

    template<typename T, glm::qualifier Q>
    struct RelativePosition<3, T, Q> : public glm::vec<3, T, Q>
    {
        constexpr static int pcs = 3;
        using Parent = glm::vec<pcs, T, Q>;
        using glm::vec<pcs, T, Q>::vec;
        using Global = GlobalPosition<pcs, T, Q>;

        constexpr explicit RelativePosition(const Parent& value)
            : Parent(value)
        {
        }

        [[nodiscard]] constexpr Global castToGlobal() const
        {
            return Global{ this->x, this->y, this->z };
        }

        [[nodiscard]] constexpr Global toGlobal(const Global& origin) const
        {
            return Global{ origin + castToGlobal() };
        }
        [[nodiscard]] constexpr RelativePosition toGlobal(const RelativePosition& origin) const
        {
            return RelativePosition{ origin + *this };
        }
    };

    using RelativePosition3F = RelativePosition<3, float>;
    using RelativePosition2F = RelativePosition<2, float>;
    using RelativePosition3D = RelativePosition<3, double>;
    using RelativePosition2D = RelativePosition<2, double>;
    using RelativePosition3I = RelativePosition<3, int>;
    using RelativePosition2I = RelativePosition<2, int>;

} // namespace Core
