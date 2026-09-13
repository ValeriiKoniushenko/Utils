// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "Core/Enum.h"

#include <gtest/gtest.h>

// clang-format off
// tag::enum_declaration[]
CreateEnum(Color, int,
    Red,
    Green,
    Blue
);
// end::enum_declaration[]

CreateEnum(Name, int,
    Jim,
    Kary = 100,
    Blue
);

#define VAL1 555
#define VAL2 444
CreateEnum(Num, int,
    None,
    Big = VAL1,
    Small = VAL2
);
// clang-format on

class EnumTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        oldCerr = std::cerr.rdbuf(capturedCerr.rdbuf()); // redirect
        oldCout = std::cout.rdbuf(capturedCout.rdbuf()); // redirect
    }
    void TearDown() override
    {
        std::cerr.rdbuf(oldCerr);
        std::cout.rdbuf(oldCout);
    }

private:
    std::ostringstream capturedCout;
    std::ostringstream capturedCerr;
    std::streambuf* oldCerr = nullptr;
    std::streambuf* oldCout = nullptr;
};

TEST_F(EnumTest, Color_Test)
{
    // tag::enum_conversion[]
    Color color = Color::Red;
    EXPECT_EQ("Red", color.toStr());
    EXPECT_EQ(0, color.cast());

    color = Color::Blue;
    EXPECT_EQ("Blue", color.toStr());
    EXPECT_EQ(2, color.cast());
    EXPECT_EQ(Color::fromStr("Blue").value_or(999), color.cast());
    EXPECT_EQ(Color::fromStr("Blue").value_or(999), 2);

    Color color2 = Color::Red;
    color = color2;
    EXPECT_EQ(color, color2);
    EXPECT_EQ("Red", color.toStr());
    EXPECT_EQ(0, color.cast());
    // end::enum_conversion[]
}

TEST_F(EnumTest, Name_Test)
{
    Name jim = Name::Jim;
    EXPECT_EQ("Jim", jim.toStr());
    EXPECT_EQ(0, jim.cast());
    EXPECT_EQ(Name::fromStr("Jim").value_or(999), jim.cast());

    Name kary = Name::Kary;
    EXPECT_EQ("Kary", kary.toStr());
    EXPECT_EQ(100, kary.cast());
    EXPECT_EQ(Name::fromStr("Kary").value_or(999), kary.cast());

    Name blue = Name::Blue;
    EXPECT_EQ("Blue", blue.toStr());
    EXPECT_EQ(101, blue.cast());
    EXPECT_EQ(Name::fromStr("Blue").value_or(999), blue.cast());
}

TEST_F(EnumTest, Num_Test)
{
    Num none = Num::None;
    EXPECT_EQ("None", none.toStr());
    EXPECT_EQ(0, none.cast());
    EXPECT_EQ(Num::fromStr("None").value_or(999), none.cast());

    Num big = Num::Big;
    EXPECT_EQ("", big.toStr());
    EXPECT_EQ(VAL1, big.cast());
    EXPECT_EQ(Num::fromStr("Big").value_or(999), 999);

    Num small = Num::Small;
    EXPECT_EQ("", small.toStr());
    EXPECT_EQ(VAL2, small.cast());
    EXPECT_EQ(Num::fromStr("Small").value_or(999), 999);
    EXPECT_EQ(Num::fromStr("Small").value_or(999), 999);
    EXPECT_EQ(Num::fromStr("Small").value_or(999), 999);
    EXPECT_EQ(Num::fromStr("Small").value_or(999), 999);
    EXPECT_EQ(Num::fromStr("Small").value_or(999), 999);
}
