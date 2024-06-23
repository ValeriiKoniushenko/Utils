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

#include "Utils/CopyableAndMoveable.h"
#include "Utils/Concepts.h"
#include "Utils.h"

#include "glm/glm.hpp"

namespace Core
{
	template<Utils::IsArithmetic T, int Dimension> struct Size final
	{
		static_assert(false, "Invalid 'Dimension' value. You can use only 2D or 3D Size type.");
	};

	template<Utils::IsArithmetic T>
	struct Size<T, 2> final
	{
		constexpr static int DimensionValue = 2;
		using ValueType = T;
		using ObjectType = Size<ValueType, DimensionValue>;
		using GlmObjectType = glm::vec<DimensionValue, T, glm::highp>;

		T width{};
		T height{};

		[[nodiscard]] constexpr bool operator<(const ObjectType& other) const noexcept
		{
			return area() < other.area();
		}

		[[nodiscard]] constexpr bool operator>(const ObjectType& other) const noexcept
		{
			return area() > other.area();
		}

		[[nodiscard]] constexpr bool operator<=(const ObjectType& other) const noexcept
		{
			return area() <= other.area();
		}

		[[nodiscard]] constexpr bool operator>=(const ObjectType& other) const noexcept
		{
			return area() >= other.area();
		}

		template<Utils::IsIntegral Type>
		[[nodiscard, maybe_unused]] constexpr bool operator==(const Size<Type, DimensionValue>& other) const noexcept
		{
			return area() == other.area();
		}

		template<Utils::IsFloating Type>
		[[nodiscard, maybe_unused]] constexpr bool operator==(const Size<Type, DimensionValue>& other) const noexcept
		{
			return IsEqual(area(), other.area());
		}

		[[nodiscard]] constexpr T area() const noexcept
		{
			return width * height;
		}

		[[nodiscard]] constexpr ObjectType operator+(const ObjectType& value) const noexcept
		{
			return { width + value.width, height + value.height };
		}

		[[nodiscard]] constexpr ObjectType operator+(T offset) const noexcept
		{
			return { width + offset, height + offset };
		}

		[[nodiscard]] constexpr ObjectType operator-(const ObjectType& value) const noexcept
		{
			return { width - value.width, height - value.height };
		}

		[[nodiscard]] constexpr ObjectType operator-(T offset) const noexcept
		{
			return { width - offset, height - offset };
		}

		[[nodiscard]] constexpr ObjectType operator*(const ObjectType& value) const
		{
			return { width * value.width, height * value.height };
		}

		[[nodiscard]] constexpr ObjectType operator*(T offset) const
		{
			return { width * offset, height * offset };
		}

		[[nodiscard]] constexpr ObjectType operator/(const ObjectType& value) const
		{
			return { width / value.width, height / value.height };
		}

		[[nodiscard]] constexpr ObjectType operator/(T offset) const
		{
			return { width / offset, height / offset };
		}

		constexpr ObjectType& operator+=(const ObjectType& other) noexcept
		{
			width += other.width;
			height += other.height;
			return *this;
		}

		constexpr ObjectType& operator+=(T offset) noexcept
		{
			width += offset;
			height += offset;
			return *this;
		}

		constexpr ObjectType& operator-=(const ObjectType& other) noexcept
		{
			width -= other.width;
			height -= other.height;
			return *this;
		}

		constexpr ObjectType& operator-=(T offset) noexcept
		{
			width -= offset;
			height -= offset;
			return *this;
		}

		template<class CastType>
		[[nodiscard]] explicit constexpr operator CastType() const noexcept
		{
			return {
				static_cast<CastType::ValueType>(width),
				static_cast<CastType::ValueType>(height)
			};
		}

		[[nodiscard]] constexpr GlmObjectType toGlm() const noexcept
		{
			return { width, height };
		}

		template<Utils::IsArithmetic Type>
		[[nodiscard]] constexpr Size<Type, DimensionValue> asSize() const noexcept
		{
			return { width, height };
		}

		[[nodiscard]] constexpr static ObjectType fromGlm(const GlmObjectType& vec) noexcept
		{
			return { vec.x, vec.y };
		}
	};

	template<Utils::IsArithmetic T>
	struct Size<T, 3> final
	{
		constexpr static int DimensionValue = 3;
		using ValueType = T;
		using ObjectType = Size<ValueType, DimensionValue>;
		using GlmObjectType = glm::vec<DimensionValue, T, glm::highp>;

		T width{};
		T height{};
		T deep{};

		[[nodiscard]] constexpr bool operator<(const ObjectType& other) const noexcept
		{
			return area() < other.area();
		}

		[[nodiscard]] constexpr bool operator>(const ObjectType& other) const noexcept
		{
			return area() > other.area();
		}

		[[nodiscard]] constexpr bool operator<=(const ObjectType& other) const noexcept
		{
			return area() <= other.area();
		}

		[[nodiscard]] constexpr bool operator>=(const ObjectType& other) const noexcept
		{
			return area() >= other.area();
		}

		template<Utils::IsIntegral Type>
		[[nodiscard, maybe_unused]] constexpr bool operator==(const Size<Type, DimensionValue>& other) const noexcept
		{
			return area() == other.area();
		}

		template<Utils::IsFloating Type>
		[[nodiscard, maybe_unused]] constexpr bool operator==(const Size<Type, DimensionValue>& other) const noexcept
		{
			return IsEqual(area(), other.area());
		}

		[[nodiscard]] constexpr T area() const noexcept
		{
			return width * height * deep;
		}

		[[nodiscard]] constexpr ObjectType operator+(const ObjectType& value) const noexcept
		{
			return { width + value.width, height + value.height, deep + value.deep };
		}

		[[nodiscard]] constexpr ObjectType operator+(T offset) const noexcept
		{
			return { width + offset, height + offset, deep + offset };
		}

		[[nodiscard]] constexpr ObjectType operator-(const ObjectType& value) const noexcept
		{
			return { width - value.width, height - value.height, deep + value.deep };
		}

		[[nodiscard]] constexpr ObjectType operator-(T offset) const noexcept
		{
			return { width - offset, height - offset, deep + offset };
		}

		[[nodiscard]] constexpr ObjectType operator*(const ObjectType& value) const
		{
			return { width * value.width, height * value.height, deep + value.deep };
		}

		[[nodiscard]] constexpr ObjectType operator*(T offset) const
		{
			return { width * offset, height * offset, deep + offset };
		}

		[[nodiscard]] constexpr ObjectType operator/(const ObjectType& value) const
		{
			return { width / value.width, height / value.height, deep + value.deep };
		}

		[[nodiscard]] constexpr ObjectType operator/(T offset) const
		{
			return { width / offset, height / offset, deep + offset };
		}

		constexpr ObjectType& operator+=(const ObjectType& other) noexcept
		{
			width += other.width;
			height += other.height;
			deep += other.deep;
			return *this;
		}

		constexpr ObjectType& operator+=(T offset) noexcept
		{
			width += offset;
			height += offset;
			deep += offset;
			return *this;
		}

		constexpr ObjectType& operator-=(const ObjectType& other) noexcept
		{
			width -= other.width;
			height -= other.height;
			deep -= other.deep;
			return *this;
		}

		constexpr ObjectType& operator-=(T offset) noexcept
		{
			width -= offset;
			height -= offset;
			deep -= offset;
			return *this;
		}

		template<class CastType>
		[[nodiscard]] explicit constexpr operator CastType() const noexcept
		{
			return {
				static_cast<CastType::ValueType>(width),
				static_cast<CastType::ValueType>(height),
				static_cast<CastType::ValueType>(deep)
			};
		}

		[[nodiscard]] constexpr GlmObjectType toGlm() const noexcept
		{
			return { width, height, deep };
		}

		template<Utils::IsArithmetic Type>
		[[nodiscard]] constexpr Size<Type, DimensionValue> asSize() const noexcept
		{
			return { width, height, deep };
		}

		[[nodiscard]] constexpr static ObjectType fromGlm(const GlmObjectType& vec) noexcept
		{
			return { vec.x, vec.y, vec.z };
		}
	};

	using FSize2 = Size<float, 2>;
	using DSize2 = Size<double, 2>;
	using ISize2 = Size<int, 2>;

	using FSize3 = Size<float, 3>;
	using DSize3 = Size<double, 3>;
	using ISize3 = Size<int, 3>;

} // namespace Core