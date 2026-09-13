// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

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

    {
        GPos3 pos1(2);
        GPos3 pos2(2);

        GPos3 pos = GPos3(pos1 + pos2);

        EXPECT_EQ(4, pos.x);
        EXPECT_EQ(4, pos.y);
        EXPECT_EQ(4, pos.z);
    }

    {
        GPos3 pos1(2);

        GPos3 pos = GPos3(glm::vec3(2) + pos1);

        EXPECT_EQ(4, pos.x);
        EXPECT_EQ(4, pos.y);
        EXPECT_EQ(4, pos.z);
    }
}

TEST(PositionTests3, RelativePositionsSimple)
{
    {
        // tag::relative_position[]
        using GPos3 = Core::GlobalPosition3F;
        using RPos3 = Core::RelativePosition3F;

        GPos3 orig(3);
        RPos3 rel(-1);

        GPos3 final = rel.toGlobal(orig);

        float commonResult = 2;
        EXPECT_EQ(commonResult, final.x);
        EXPECT_EQ(commonResult, final.y);
        EXPECT_EQ(commonResult, final.z);
        // end::relative_position[]
    }

    {
        GPos3 orig(3);
        RPos3 rel1(-1);
        RPos3 rel2(-1);

        {
            RPos3 final = rel2.toGlobal(rel1);
            float commonResult = -2;
            EXPECT_EQ(commonResult, final.x);
            EXPECT_EQ(commonResult, final.y);
            EXPECT_EQ(commonResult, final.z);
        }

        {
            GPos3 final = rel2.toGlobal(rel1).toGlobal(orig);
            float commonResult = 1;
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
        GPos2 orig(3);
        RPos2 rel(-1);

        GPos2 final = rel.toGlobal(orig);

        float commonResult = 2;
        EXPECT_EQ(commonResult, final.x);
        EXPECT_EQ(commonResult, final.y);
    }

    {
        GPos2 orig(3);
        RPos2 rel1(-1);
        RPos2 rel2(-1);

        {
            RPos2 final = rel2.toGlobal(rel1);
            float commonResult = -2;
            EXPECT_EQ(commonResult, final.x);
            EXPECT_EQ(commonResult, final.y);
        }

        {
            GPos2 final = rel2.toGlobal(rel1).toGlobal(orig);
            float commonResult = 1;
            EXPECT_EQ(commonResult, final.x);
            EXPECT_EQ(commonResult, final.y);
        }
    }
}
