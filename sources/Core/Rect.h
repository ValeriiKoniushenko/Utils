// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "BaseAssert.h"
#include "Position.h"
#include "Size.h"
#include "Utils/Concepts.h"

namespace Core
{
    template<Utils::IsArithmetic T>
    class Rect final
    {
    public:
        constexpr static int DimensionValue = 2;
        using GlobalPositionT = GlobalPosition<DimensionValue, T>;
        using SizeT = Size<T, DimensionValue>;

    public:
        constexpr Rect() = default;
        constexpr Rect(const Rect&) = default;
        constexpr Rect(Rect&&) noexcept = default;
        Rect& operator=(const Rect&) = default;
        Rect& operator=(Rect&&) noexcept = default;

        constexpr Rect(T left, T top, T right, T bottom)
            : _left(left),
              _right(right),
              _bottom(bottom),
              _top(top)
        {
#ifdef CORE_DEBUG
            updateDebugData();
#endif
        }

        constexpr Rect(GlobalPositionT leftTop, GlobalPositionT rightBottom)
            : Rect(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y)
        {
        }

        constexpr Rect(GlobalPositionT leftTop, const SizeT& size)
            : Rect(leftTop.x, leftTop.y, leftTop.x + size.width, leftTop.y - size.height)
        {
        }

        constexpr void setLeftTop(const GlobalPositionT& leftTop) noexcept
        {
#ifdef CORE_DEBUG
            updateDebugData();
            (void)requireValid();
#endif
            _left = leftTop.x;
            _top = leftTop.y;
        }

        constexpr void setLeftBottom(const GlobalPositionT& leftBottom) noexcept
        {
#ifdef CORE_DEBUG
            updateDebugData();
#endif
            _left = leftBottom.x;
            _bottom = leftBottom.y;
        }

        constexpr void setRightBottom(const GlobalPositionT& rightBottom) noexcept
        {
#ifdef CORE_DEBUG
            updateDebugData();
#endif
            _right = rightBottom.x;
            _bottom = rightBottom.y;
        }

        constexpr void setRightTop(const GlobalPositionT& rightTop) noexcept
        {
#ifdef CORE_DEBUG
            updateDebugData();
#endif
            _right = rightTop.x;
            _top = rightTop.y;
        }

        [[nodiscard]] constexpr T getWidth() const noexcept { return ::Math::Abs(_right - _left); }

        [[nodiscard]] constexpr T getHeight() const noexcept { return ::Math::Abs(_top - _bottom); }

        [[nodiscard]] constexpr GlobalPositionT getLeftTop() const noexcept
        {
            return GlobalPositionT{ _left, _top };
        }

        [[nodiscard]] constexpr GlobalPositionT getLeftBottom() const noexcept
        {
            return GlobalPositionT{ _left, _bottom };
        }

        [[nodiscard]] constexpr GlobalPositionT getRightBottom() const noexcept
        {
            return GlobalPositionT{ _right, _bottom };
        }

        [[nodiscard]] constexpr GlobalPositionT getRightTop() const noexcept
        {
            return GlobalPositionT{ _right, _top };
        }

        [[nodiscard]] constexpr GlobalPositionT getCenter() const noexcept
        {
            return GlobalPositionT{ (_left + _right) / static_cast<T>(2),
                                    (_bottom + _top) / static_cast<T>(2) };
        }

        [[nodiscard]] constexpr bool isValid() const noexcept
        {
            return _top >= _bottom && _left <= _right;
        }

        bool requireValid() const noexcept { return Verify(isValid()); }

        [[nodiscard]] constexpr bool isContain(const GlobalPositionT& point) const noexcept
        {
            return point.x >= _left && point.x <= _right && point.y <= _top && point.y >= _bottom;
        }

        [[nodiscard]] constexpr bool isContain(const Rect<T>& rect) const noexcept
        {
            return isContain(rect.getLeftBottom()) || isContain(rect.getLeftTop())
                   || isContain(rect.getRightBottom()) || isContain(rect.getRightTop());
        }

    private:
        /// @brief global position
        T _left{};
        /// @brief global position
        T _right{};
        /// @brief global position
        T _bottom{};
        /// @brief global position
        T _top{};

#ifdef CORE_DEBUG
        constexpr void updateDebugData()
        {
            _width = getWidth();
            _height = getHeight();
        }

        T _width{};
        T _height{};
#endif
    };

    using FRect = Rect<float>;
    using DRect = Rect<double>;
    using IRect = Rect<int>;

} // namespace Core
