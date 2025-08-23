// MIT License
//
// Copyright (c) 2024 Valerii Koniushenko
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

#include "Core/Color.h"

#include <gtest/gtest.h>

using Core::Color3;
using Core::Color4;
using Core::NormColor3;
using Core::NormColor4;

TEST(Color4Test, ConstructFromParent)
{
    {
        Color4 c(glm::u8vec4(10, 20, 30, 40));
        EXPECT_EQ(c.x, 10);
        EXPECT_EQ(c.y, 20);
        EXPECT_EQ(c.z, 30);
        EXPECT_EQ(c.w, 40);
    }

    {
        constexpr Color4 c(glm::u8vec4(10, 20, 30, 40));
        EXPECT_EQ(c.x, 10);
        EXPECT_EQ(c.y, 20);
        EXPECT_EQ(c.z, 30);
        EXPECT_EQ(c.w, 40);
    }
}

TEST(Color4Test, FromNormColor4)
{
    {
        NormColor4 n(glm::vec4(0.0f, 0.5f, 1.0f, 0.25f));
        Color4 c = Color4::From(n);
        EXPECT_EQ(c.x, 0);
        EXPECT_EQ(c.y, 127);
        EXPECT_EQ(c.z, 255);
        EXPECT_EQ(c.w, 63);
    }

    {
        constexpr NormColor4 n(glm::vec4(0.0f, 0.5f, 1.0f, 0.25f));
        Color4 c = Color4::From(n);
        EXPECT_EQ(c.x, 0);
        EXPECT_EQ(c.y, 127);
        EXPECT_EQ(c.z, 255);
        EXPECT_EQ(c.w, 63);
    }
}

TEST(Color4Test, FromNormColor4Mutate)
{
    NormColor4 n(glm::vec4(1.0f, 0.0f, 0.5f, 1.0f));
    Color4 c(glm::u8vec4(0, 0, 0, 0));
    c.from(n);
    EXPECT_EQ(c, Color4::From(n));
}

TEST(NormColor4Test, FromColor4)
{
    Color4 c(glm::u8vec4(0, 128, 255, 64));
    NormColor4 n = NormColor4::From(c);
    EXPECT_FLOAT_EQ(n.x, 0.0f);
    EXPECT_NEAR(n.y, 128.0f / 255.0f, 1e-6);
    EXPECT_FLOAT_EQ(n.z, 1.0f);
    EXPECT_NEAR(n.w, 64.0f / 255.0f, 1e-6);
}

TEST(NormColor4Test, FromColor4Mutate)
{
    Color4 c(glm::u8vec4(255, 127, 0, 200));
    NormColor4 n(glm::vec4(0.0f));
    n.from(c);
    EXPECT_EQ(n, NormColor4::From(c));
}

TEST(Color4RoundTrip, BackAndForth)
{
    Color4 c1(glm::u8vec4(10, 127, 250, 99));
    NormColor4 n = NormColor4::From(c1);
    Color4 c2 = Color4::From(n);

    // Round-trip should be close; allow 1 off due to rounding
    EXPECT_NEAR(c1.x, c2.x, 1);
    EXPECT_NEAR(c1.y, c2.y, 1);
    EXPECT_NEAR(c1.z, c2.z, 1);
    EXPECT_NEAR(c1.w, c2.w, 1);
}

// ---------- Color3 <-> NormColor3 ----------
TEST(Color3Test, ConstructFromParent)
{
    {
        Color3 c(glm::u8vec3(1, 2, 3));
        EXPECT_EQ(c.x, 1);
        EXPECT_EQ(c.y, 2);
        EXPECT_EQ(c.z, 3);
    }

    {
        constexpr Color3 c(glm::u8vec3(1, 2, 3));
        EXPECT_EQ(c.x, 1);
        EXPECT_EQ(c.y, 2);
        EXPECT_EQ(c.z, 3);
    }
}

TEST(Color3Test, FromNormColor3)
{
    {
        NormColor3 n(glm::vec3(0.0f, 0.5f, 1.0f));
        Color3 c = Color3::From(n);
        EXPECT_EQ(c.x, 0);
        EXPECT_EQ(c.y, 127);
        EXPECT_EQ(c.z, 255);
    }

    {
        constexpr NormColor3 n(glm::vec3(0.0f, 0.5f, 1.0f));
        Color3 c = Color3::From(n);
        EXPECT_EQ(c.x, 0);
        EXPECT_EQ(c.y, 127);
        EXPECT_EQ(c.z, 255);
    }
}

TEST(Color3Test, FromNormColor3Mutate)
{
    NormColor3 n(glm::vec3(0.25f, 0.75f, 1.0f));
    Color3 c(glm::u8vec3(0, 0, 0));
    c.from(n);
    EXPECT_EQ(c, Color3::From(n));
}

TEST(NormColor3Test, FromColor3)
{
    Color3 c(glm::u8vec3(0, 128, 255));
    NormColor3 n = NormColor3::From(c);
    EXPECT_FLOAT_EQ(n.x, 0.0f);
    EXPECT_NEAR(n.y, 128.0f / 255.0f, 1e-6);
    EXPECT_FLOAT_EQ(n.z, 1.0f);
}

TEST(NormColor3Test, FromColor3Mutate)
{
    Color3 c(glm::u8vec3(200, 100, 50));
    NormColor3 n(glm::vec3(0.0f));
    n.from(c);
    EXPECT_EQ(n, NormColor3::From(c));
}

TEST(Color3RoundTrip, BackAndForth)
{
    Color3 c1(glm::u8vec3(42, 128, 250));
    NormColor3 n = NormColor3::From(c1);
    Color3 c2 = Color3::From(n);

    EXPECT_NEAR(c1.x, c2.x, 1);
    EXPECT_NEAR(c1.y, c2.y, 1);
    EXPECT_NEAR(c1.z, c2.z, 1);
}