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

#include "Core/Size.h"

#include <gtest/gtest.h>

TEST(CoreTests, Size2Area)
{
    {
        constexpr Core::FSize2 _;
    }
    {
        constexpr float v = 5.5f;
        constexpr auto a = Core::Size<decltype(v), 2>{ v, v };
        static_assert(a.area() == v * v);
    }

    {
        constexpr int v = 5;
        constexpr auto a = Core::Size<decltype(v), 2>{ v, v };
        static_assert(a.area() == v * v);
    }
}

TEST(CoreTests, Size2LessGreater)
{
    {
        constexpr float v1 = 5.5f, v2 = 3.3f;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        static_assert(b < a);
        static_assert(a > b);
        ASSERT_TRUE(b < a);
        ASSERT_TRUE(a > b);

        constexpr auto a1 = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b1 = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        static_assert(b1 <= a1);
        static_assert(a1 >= b1);
        ASSERT_TRUE(b1 <= a1);
        ASSERT_TRUE(a1 >= b1);
    }

    {
        constexpr int v1 = 5, v2 = 3;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        static_assert(b < a);
        static_assert(a > b);
        ASSERT_TRUE(b < a);
        ASSERT_TRUE(a > b);

        constexpr auto a1 = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b1 = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        static_assert(b1 <= a1);
        static_assert(a1 >= b1);
        ASSERT_TRUE(b1 <= a1);
        ASSERT_TRUE(a1 >= b1);
    }
}

TEST(CoreTests, Size2Equal)
{
    {
        constexpr float v1 = 5.5f;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        static_assert(a == b);
        ASSERT_TRUE(a == b);
    }

    {
        constexpr int v1 = 5;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        static_assert(a == b);
        ASSERT_TRUE(a == b);
    }
}

TEST(CoreTests, Size2NotEqual)
{
    {
        constexpr float v1 = 5.5f, v2 = 3.3f;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        static_assert(a != b);
        ASSERT_TRUE(a != b);
    }

    {
        constexpr int v1 = 5, v2 = 3;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        static_assert(a != b);
        ASSERT_TRUE(a != b);
    }
}

TEST(CoreTests, Size2Sum)
{
    {
        constexpr float v1 = 5.5f, v2 = 3.3f;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        constexpr auto result = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 + v2, v1 + v2 };
        static_assert(a + b == result);
        ASSERT_TRUE(a + b == result);
    }

    {
        constexpr int v1 = 5, v2 = 3;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        static_assert(a + b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 + v2, v1 + v2 });
        ASSERT_TRUE((a + b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 + v2, v1 + v2 }));
    }
}

TEST(CoreTests, Size2Substract)
{
    {
        constexpr float v1 = 5.5f, v2 = 3.3f;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        constexpr auto result = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 - v2, v1 - v2 };
        static_assert(a - b == result);
        ASSERT_TRUE(a - b == result);
    }

    {
        constexpr int v1 = 5, v2 = 3;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        static_assert(a - b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 - v2, v1 - v2 });
        ASSERT_TRUE((a - b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 - v2, v1 - v2 }));
    }
}

TEST(CoreTests, Size2Multiplication)
{
    {
        constexpr float v1 = 5.5f, v2 = 3.3f;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        constexpr auto result = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 * v2, v1 * v2 };
        static_assert(a * b == result);
        ASSERT_TRUE(a * b == result);
    }

    {
        constexpr int v1 = 5, v2 = 3;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        static_assert(a * b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 * v2, v1 * v2 });
        ASSERT_TRUE((a * b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 * v2, v1 * v2 }));
    }
}

TEST(CoreTests, Size2Dividing)
{
    {
        constexpr float v1 = 5.5f, v2 = 3.3f;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        constexpr auto result = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 / v2, v1 / v2 };
        static_assert(a / b == result);
        ASSERT_TRUE(a / b == result);
    }

    {
        constexpr int v1 = 5, v2 = 3;
        constexpr auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        constexpr auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        static_assert(a / b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 / v2, v1 / v2 });
        ASSERT_TRUE((a / b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 / v2, v1 / v2 }));
    }
}

TEST(CoreTests, Size2Appending)
{
    {
        const float v1 = 5.5f, v2 = 3.3f;
        auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        const auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        auto correct = a + b;
        EXPECT_EQ(correct, (a += b));
    }

    {
        const float v1 = 5.5f;
        auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        auto correct = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 + 1.f, v1 + 1.f };
        a += 1.f;
        EXPECT_EQ(correct, a);
    }

    {
        constexpr int v1 = 5, v2 = 3;
        auto a = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1, v1 };
        auto b = Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v2, v2 };
        auto correct = a + b;

        EXPECT_EQ(correct, (a += b));
    }
}

TEST(CoreTests, SizeCasting)
{
    {
        constexpr Core::FSize2 fsize = Core::FSize2{ 2.5f, 5.5f };
        constexpr Core::ISize2 isize = static_cast<Core::ISize2>(fsize);

        static_assert(2.f * 5.f == isize.area());
    }

    {
        constexpr glm::vec2 vec(2, 5);
        constexpr glm::vec2 gvec = Core::FSize2{ 2, 5 }.toGlm();
        static_assert(vec == gvec);
    }

    {
        constexpr auto size = Core::FSize2::fromGlm(glm::vec2(1.f, 2.f));
        static_assert(std::is_same_v<Core::FSize2, std::remove_cv<decltype(size)>::type>);
        static_assert(size.width == 1.f && size.height == 2.f);
    }
}