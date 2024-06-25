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

#include "Utils/Concepts.h"
#include "Utils/CopyableAndMoveableBehaviour.h"
#include "Size.h"
#include "Position.h"
#include "Assert.h"

namespace Core
{
	template<Utils::IsArithmetic T>
	class Rect final : public Utils::CopyableAndMoveable
	{
	public:
		constexpr static int DimensionValue = 2;
		using GlobalPositionT = GlobalPosition<DimensionValue, T>;
		using SizeT = Size<T, DimensionValue>;

	public:
		constexpr Rect() = default;
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
			requireValid();
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

		[[nodiscard]] constexpr T getWidth() const noexcept
		{
			return Math::Abs(_right - _left);
		}

		[[nodiscard]] constexpr T getHeight() const noexcept
		{
			return Math::Abs(_top - _bottom);
		}

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
			return GlobalPositionT{
				(_left + _right) / static_cast<T>(2),
				(_bottom + _top) / static_cast<T>(2)
			};
		}

		[[nodiscard]] constexpr bool isValid() const noexcept
		{
			return _top >= _bottom && _left <= _right;
		}

		bool requireValid() const noexcept
		{
			if (isValid())
			{
				return true;
			}

			Assert(false);

			return false;
		}

		[[nodiscard]] constexpr bool isContain(const GlobalPositionT& point) const noexcept
		{
			return point.x >= _left && point.x <= _right &&
				point.y <= _top && point.y >= _bottom;
		}

		[[nodiscard]] constexpr bool isContain(const Rect<T>& rect) const noexcept
		{
			return isContain(rect.getLeftBottom()) || isContain(rect.getLeftTop()) || isContain(rect.getRightBottom()) || isContain(rect.getRightTop());
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
