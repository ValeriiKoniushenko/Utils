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

#pragma once

#include "Math.h"
#include "Utils/Concepts.h"
#include "glm/glm.hpp"

namespace Core
{
    template<Utils::IsArithmetic, int Dimension>
    /**
     * @class Size
     * @brief Base template for size classes representing 2D or 3D dimensions.
     *
     * This is the base template class that is specialized for 2D and 3D dimensions.
     * It cannot be instantiated directly - only the specializations for 2D and 3D
     * can be used.
     *
     * @tparam T The arithmetic type used for the dimensions (float, double, int etc.)
     * @tparam Dimension The dimension of the size (2 or 3)
     */
    struct Size final
    {
        static_assert(!!Dimension,
                      "Invalid 'Dimension' value. You can use only 2D or 3D Size type.");
    };

    template<Utils::IsArithmetic T>
    /**
     * @class Size<T,2>
     * @brief Specialized Size class for 2D dimensions.
     *
     * Represents 2D dimensions with width and height components. Provides arithmetic
     * operations, comparison operators, and conversion utilities.
     *
     * @tparam T The arithmetic type used for dimensions (float, double, int etc.)
     */
    struct Size<T, 2> final
    {
        constexpr static int DimensionValue = 2;
        using ValueType = T;
        using ObjectType = Size<ValueType, DimensionValue>;
        using GlmObjectType = glm::vec<DimensionValue, T, glm::highp>;

        Size() = default;

        /**
         * @brief Constructs a Size object with equal width and height.
         * @param scalar The value to set for both width and height
         */
        constexpr explicit Size(T scalar)
            : width{ scalar },
              height{ scalar }
        {
        }

        /**
         * @brief Constructs a Size object with specified width and height.
         * @param w Width value
         * @param h Height value
         */
        constexpr explicit Size(T w, T h)
            : width{ w },
              height{ h }
        {
        }

        constexpr explicit Size(GlmObjectType value)
            : width{ value.x },
              height{ value.y }
        {
        }

        T width{};
        T height{};

        /**
         * @brief Compares if this Size's area is less than another's.
         * @param other Size to compare against
         * @return true if this Size's area is less than other's
         */
        [[nodiscard]] constexpr bool operator<(const ObjectType& other) const noexcept
        {
            return area() < other.area();
        }

        /**
         * @brief Compares if this Size's area is greater than another's.
         * @param other Size to compare against
         * @return true if this Size's area is greater than other's
         */
        [[nodiscard]] constexpr bool operator>(const ObjectType& other) const noexcept
        {
            return area() > other.area();
        }

        /**
         * @brief Compares if this Size's area is less than or equal to another's.
         * @param other Size to compare against
         * @return true if this Size's area is less than or equal to other's
         */
        [[nodiscard]] constexpr bool operator<=(const ObjectType& other) const noexcept
        {
            return area() <= other.area();
        }

        /**
         * @brief Compares if this Size's area is greater than or equal to another's.
         * @param other Size to compare against
         * @return true if this Size's area is greater than or equal to other's
         */
        [[nodiscard]] constexpr bool operator>=(const ObjectType& other) const noexcept
        {
            return area() >= other.area();
        }

        /**
         * @brief Equality comparison for integral types.
         * @tparam Type The integral type to compare with
         * @param other Size to compare against
         * @return true if areas are exactly equal
         */
        template<Utils::IsIntegral Type>
        [[nodiscard, maybe_unused]] constexpr bool operator==(
            const Size<Type, DimensionValue>& other) const noexcept
        {
            return area() == other.area();
        }

        /**
         * @brief Equality comparison for floating point types.
         * @tparam Type The floating point type to compare with
         * @param other Size to compare against
         * @return true if areas are approximately equal
         */
        template<Utils::IsFloating Type>
        [[nodiscard, maybe_unused]] constexpr bool operator==(
            const Size<Type, DimensionValue>& other) const noexcept
        {
            return ::Math::IsEqual(area(), other.area());
        }

        [[nodiscard]] constexpr T area() const noexcept { return width * height; }

        /**
         * @brief Adds corresponding dimensions of two Size objects.
         * @param value Size to add
         * @return New Size with added dimensions
         */
        [[nodiscard]] constexpr ObjectType operator+(const ObjectType& value) const noexcept
        {
            return ObjectType{ width + value.width, height + value.height };
        }

        /**
         * @brief Adds a scalar value to both dimensions.
         * @param offset Value to add to both dimensions
         * @return New Size with added offset
         */
        [[nodiscard]] constexpr ObjectType operator+(T offset) const noexcept
        {
            return ObjectType{ width + offset, height + offset };
        }

        /**
         * @brief Subtracts corresponding dimensions of two Size objects.
         * @param value Size to subtract
         * @return New Size with subtracted dimensions
         */
        [[nodiscard]] constexpr ObjectType operator-(const ObjectType& value) const noexcept
        {
            return ObjectType{ width - value.width, height - value.height };
        }

        /**
         * @brief Subtracts a scalar value from both dimensions.
         * @param offset Value to subtract from both dimensions
         * @return New Size with subtracted offset
         */
        [[nodiscard]] constexpr ObjectType operator-(T offset) const noexcept
        {
            return { width - offset, height - offset };
        }

        /**
         * @brief Multiplies corresponding dimensions of two Size objects.
         * @param value Size to multiply by
         * @return New Size with multiplied dimensions
         */
        [[nodiscard]] constexpr ObjectType operator*(const ObjectType& value) const
        {
            return ObjectType{ width * value.width, height * value.height };
        }

        /**
         * @brief Multiplies both dimensions by a scalar value.
         * @param offset Value to multiply both dimensions by
         * @return New Size with multiplied dimensions
         */
        [[nodiscard]] constexpr ObjectType operator*(T offset) const
        {
            return ObjectType{ width * offset, height * offset };
        }

        /**
         * @brief Divides corresponding dimensions by another Size object.
         * @param value Size to divide by
         * @return New Size with divided dimensions
         */
        [[nodiscard]] constexpr ObjectType operator/(const ObjectType& value) const
        {
            return ObjectType{ width / value.width, height / value.height };
        }

        /**
         * @brief Divides both dimensions by a scalar value.
         * @param offset Value to divide both dimensions by
         * @return New Size with divided dimensions
         */
        [[nodiscard]] constexpr ObjectType operator/(T offset) const
        {
            return ObjectType{ width / offset, height / offset };
        }

        /**
         * @brief Adds another Size object to this one.
         * @param other Size to add
         * @return Reference to this object
         */
        constexpr ObjectType& operator+=(const ObjectType& other) noexcept
        {
            width += other.width;
            height += other.height;
            return *this;
        }

        /**
         * @brief Adds a scalar value to both dimensions.
         * @param offset Value to add
         * @return Reference to this object
         */
        constexpr ObjectType& operator+=(T offset) noexcept
        {
            width += offset;
            height += offset;
            return *this;
        }

        /**
         * @brief Subtracts another Size object from this one.
         * @param other Size to subtract
         * @return Reference to this object
         */
        constexpr ObjectType& operator-=(const ObjectType& other) noexcept
        {
            width -= other.width;
            height -= other.height;
            return *this;
        }

        /**
         * @brief Subtracts a scalar value from both dimensions.
         * @param offset Value to subtract
         * @return Reference to this object
         */
        constexpr ObjectType& operator-=(T offset) noexcept
        {
            width -= offset;
            height -= offset;
            return *this;
        }

        template<class CastType>
        [[nodiscard]] explicit constexpr operator CastType() const noexcept
        {
            return CastType{ static_cast<typename CastType::ValueType>(width),
                             static_cast<typename CastType::ValueType>(height) };
        }

        [[nodiscard]] constexpr GlmObjectType toGlm() const noexcept { return { width, height }; }

        template<Utils::IsArithmetic Type>
        [[nodiscard]] constexpr Size<Type, DimensionValue> asSize() const noexcept
        {
            return { width, height };
        }

        [[nodiscard]] constexpr static ObjectType fromGlm(const GlmObjectType& vec) noexcept
        {
            return ObjectType{ vec.x, vec.y };
        }
    };

    template<Utils::IsArithmetic T>
    /**
     * @class Size<T,3>
     * @brief Specialized Size class for 3D dimensions.
     *
     * Represents 3D dimensions with width, height, and deep components. Provides arithmetic
     * operations, comparison operators, and conversion utilities.
     *
     * @tparam T The arithmetic type used for dimensions (float, double, int etc.)
     */
    struct Size<T, 3> final
    {
        constexpr static int DimensionValue = 3;
        using ValueType = T;
        using ObjectType = Size<ValueType, DimensionValue>;
        using GlmObjectType = glm::vec<DimensionValue, T, glm::highp>;

        Size() = default;

        constexpr explicit Size(T scalar)
            : width{ scalar },
              height{ scalar },
              deep{ scalar }
        {
        }

        constexpr explicit Size(T w, T h, T d)
            : width{ w },
              height{ h },
              deep{ d }
        {
        }

        /**
         * @brief Constructs a Size object from a GLM vector.
         * @param value GLM vector containing width (x) and height (y) components
         */
        constexpr explicit Size(GlmObjectType value)
            : width{ value.x },
              height{ value.y },
              deep{ value.z }
        {
        }

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
        [[nodiscard, maybe_unused]] constexpr bool operator==(
            const Size<Type, DimensionValue>& other) const noexcept
        {
            return area() == other.area();
        }

        template<Utils::IsFloating Type>
        [[nodiscard, maybe_unused]] constexpr bool operator==(
            const Size<Type, DimensionValue>& other) const noexcept
        {
            return IsEqual(area(), other.area());
        }

        [[nodiscard]] constexpr T area() const noexcept { return width * height * deep; }

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

        /**
         * @brief Converts this Size to another Size type.
         * @tparam CastType The target Size type
         * @return Size object converted to the target type
         */
        template<class CastType>
        [[nodiscard]] explicit constexpr operator CastType() const noexcept
        {
            return { static_cast<typename CastType::ValueType>(width),
                     static_cast<typename CastType::ValueType>(height),
                     static_cast<CastType::ValueType>(deep) };
        }

        /**
         * @brief Converts this Size to a GLM vector.
         * @return GLM vector containing the dimensions
         */
        [[nodiscard]] constexpr GlmObjectType toGlm() const noexcept
        {
            return { width, height, deep };
        }

        /**
         * @brief Converts this Size to a Size with different value type.
         * @tparam Type The target arithmetic type
         * @return Size object with converted value type
         */
        template<Utils::IsArithmetic Type>
        [[nodiscard]] constexpr Size<Type, DimensionValue> asSize() const noexcept
        {
            return { width, height, deep };
        }

        /**
         * @brief Creates a Size object from a GLM vector.
         * @param vec GLM vector to convert
         * @return New Size object with dimensions from the GLM vector
         */
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
