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

#include "Core/Enum.h"

#include <gtest/gtest.h>

// clang-format off
CreateEnum(Color, int,
    Red,
    Green,
    Blue
);

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
    Color color = Color::Red;
    EXPECT_EQ("Red", color.toStr());
    EXPECT_EQ(0, color.cast());

    color = Color::Blue;
    EXPECT_EQ("Blue", color.toStr());
    EXPECT_EQ(2, color.cast());
    EXPECT_EQ(Color::fromStr("Blue").value_or(999), color.cast());
    EXPECT_EQ(Color::fromStr("Blue").value_or(999), 2);
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
