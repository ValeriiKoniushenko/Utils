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

#include "Core/Position.h"

#include <gtest/gtest.h>

using namespace Core;

using GPos3 = GlobalPosition3F;
using RPos3 = RelativePosition3F;

TEST(PositionTests3, SimpleCreation)
{
    {
        GPos3 _;
    }

    {
        GPos3 pos(3, 1, 2);
        EXPECT_EQ(3, pos.x);
        EXPECT_EQ(1, pos.y);
        EXPECT_EQ(2, pos.z);
    }

    {
        GPos3 pos(3);
        EXPECT_EQ(3, pos.x);
        EXPECT_EQ(3, pos.y);
        EXPECT_EQ(3, pos.z);
    }
}

TEST(PositionTests3, DoubleConvestionFromToGlm)
{
    {
        GPos3 pos = GPos3(glm::vec3(3, 1, 2));
        EXPECT_EQ(3, pos.x);
        EXPECT_EQ(1, pos.y);
        EXPECT_EQ(2, pos.z);
    }

    {
        glm::vec3 pos = GPos3(3, 1, 2);
        EXPECT_EQ(3, pos.x);
        EXPECT_EQ(1, pos.y);
        EXPECT_EQ(2, pos.z);
    }
}

TEST(PositionTests3, Math)
{
    {
        GPos3 pos = GPos3(glm::vec3(3, 1, 2));

        pos += 3;

        EXPECT_EQ(3 + 3, pos.x);
        EXPECT_EQ(3 + 1, pos.y);
        EXPECT_EQ(3 + 2, pos.z);
    }

    {
        GPos3 pos = GPos3(glm::vec3(3, 1, 2));

        pos -= 3;

        EXPECT_EQ(3 - 3, pos.x);
        EXPECT_EQ(1 - 3, pos.y);
        EXPECT_EQ(2 - 3, pos.z);
    }

    {
        GPos3 pos = GPos3(glm::vec3(3, 1, 2));

        pos += glm::vec3(1, 0, 0);

        EXPECT_EQ(3 + 1, pos.x);
        EXPECT_EQ(1, pos.y);
        EXPECT_EQ(2, pos.z);
    }

    {
        GPos3 pos1(2);
        GPos3 pos2(3);

        GPos3 pos = GPos3(pos1 + pos2);

        EXPECT_EQ(5, pos.x);
        EXPECT_EQ(5, pos.y);
        EXPECT_EQ(5, pos.z);
    }

    {
        GPos3 pos1(2);

        GPos3 pos = GPos3(pos1 + glm::vec3(3));

        EXPECT_EQ(5, pos.x);
        EXPECT_EQ(5, pos.y);
        EXPECT_EQ(5, pos.z);
    }
}

TEST(PositionTests3, RelativePositionsSimple)
{
    {
        constexpr GPos3 orig(3);
        constexpr RPos3 rel(-1);

        constexpr GPos3 final = rel.toGlobal(orig);

        constexpr float commonResult = 2;
        EXPECT_EQ(commonResult, final.x);
        EXPECT_EQ(commonResult, final.y);
        EXPECT_EQ(commonResult, final.z);
    }

    {
        constexpr GPos3 orig(3);
        constexpr RPos3 rel1(-1);
        constexpr RPos3 rel2(-1);

        {
            constexpr RPos3 final = rel2.toGlobal(rel1);
            constexpr float commonResult = -2;
            EXPECT_EQ(commonResult, final.x);
            EXPECT_EQ(commonResult, final.y);
            EXPECT_EQ(commonResult, final.z);
        }

        {
            constexpr GPos3 final = rel2.toGlobal(rel1).toGlobal(orig);
            constexpr float commonResult = 1;
            EXPECT_EQ(commonResult, final.x);
            EXPECT_EQ(commonResult, final.y);
            EXPECT_EQ(commonResult, final.z);
        }
    }
}

// ============== VEC 2 ===================

using GPos2 = GlobalPosition2F;
using RPos2 = RelativePosition2F;

TEST(PositionTests2, SimpleCreation)
{
    {
        GPos2 _;
    }

    {
        GPos2 pos(3, 1);
        EXPECT_EQ(3, pos.x);
        EXPECT_EQ(1, pos.y);
    }

    {
        GPos2 pos(3);
        EXPECT_EQ(3, pos.x);
        EXPECT_EQ(3, pos.y);
    }
}

TEST(PositionTests2, DoubleConvestionFromToGlm)
{
    {
        GPos2 pos = GPos2(glm::vec2(3, 1));
        EXPECT_EQ(3, pos.x);
        EXPECT_EQ(1, pos.y);
    }

    {
        glm::vec2 pos = GPos2(3, 1);
        EXPECT_EQ(3, pos.x);
        EXPECT_EQ(1, pos.y);
    }
}

TEST(PositionTests2, Math)
{
    {
        GPos2 pos = GPos2(glm::vec2(3, 1));

        pos += 3;

        EXPECT_EQ(3 + 3, pos.x);
        EXPECT_EQ(3 + 1, pos.y);
    }

    {
        GPos2 pos = GPos2(glm::vec2(3, 1));

        pos -= 3;

        EXPECT_EQ(3 - 3, pos.x);
        EXPECT_EQ(1 - 3, pos.y);
    }

    {
        GPos2 pos = GPos2(glm::vec2(3, 1));

        pos += glm::vec2(1, 0);

        EXPECT_EQ(3 + 1, pos.x);
        EXPECT_EQ(1, pos.y);
    }

    {
        GPos2 pos1(2);
        GPos2 pos2(3);

        GPos2 pos = GPos2(pos1 + pos2);

        EXPECT_EQ(5, pos.x);
        EXPECT_EQ(5, pos.y);
    }

    {
        GPos2 pos1(2);

        GPos2 pos = GPos2(pos1 + glm::vec2(3));

        EXPECT_EQ(5, pos.x);
        EXPECT_EQ(5, pos.y);
    }
}

TEST(PositionTests2, RelativePositionsSimple)
{
    {
        constexpr GPos2 orig(3);
        constexpr RPos2 rel(-1);

        constexpr GPos2 final = rel.toGlobal(orig);

        constexpr float commonResult = 2;
        EXPECT_EQ(commonResult, final.x);
        EXPECT_EQ(commonResult, final.y);
    }

    {
        constexpr GPos2 orig(3);
        constexpr RPos2 rel1(-1);
        constexpr RPos2 rel2(-1);

        {
            constexpr RPos2 final = rel2.toGlobal(rel1);
            constexpr float commonResult = -2;
            EXPECT_EQ(commonResult, final.x);
            EXPECT_EQ(commonResult, final.y);
        }

        {
            constexpr GPos2 final = rel2.toGlobal(rel1).toGlobal(orig);
            constexpr float commonResult = 1;
            EXPECT_EQ(commonResult, final.x);
            EXPECT_EQ(commonResult, final.y);
        }
    }
}