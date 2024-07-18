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

#include "Core/String.h"

#include <gtest/gtest.h>

TEST(StringTest, BaseString_char_default__Creation)
{
    using Core::String;
    using Core::StringAtom;

    {
        StringAtom str1 = "Hello"_atom;
        StringAtom str2 = "Hello"_atom;
        StringAtom str3 = "World"_atom;
        EXPECT_EQ(str1, str2);
        EXPECT_NE(str1, str3);
    }

    {
        const StringAtom str1 = "Hello"_atom;
        const StringAtom str2 = "Hello"_atom;
        const StringAtom str3 = "World"_atom;
        EXPECT_EQ(str1, str2);
        EXPECT_NE(str1, str3);
    }

    {
        const char* dynamicStr = new char[128]{ "World" };
        const StringAtom str1 = "Hello"_atom;
        const StringAtom str2 = StringAtom::Intern(dynamicStr);
        EXPECT_NE(str1, str2);

        delete[] dynamicStr;
    }

    {
        const char* dynamicStr = new char[128]{ "World" };
        StringAtom str1 = "Hello"_atom;
        StringAtom str2 = StringAtom::Intern(dynamicStr);
        EXPECT_NE(str1, str2);

        delete[] dynamicStr;
    }

    {
        StringAtom str1 = "Hello"_atom;
        StringAtom str2 = StringAtom::Intern(std::string("World"));
        EXPECT_NE(str1, str2);
    }
}

TEST(StringTest, BaseString_char_default__Comparision)
{
    using Core::StringAtom;

    {
        const StringAtom str1 = "AAA"_atom;
        const StringAtom str2 = StringAtom::Intern(std::string("AAB"));
        EXPECT_NE(str1, str2);

        EXPECT_TRUE(str1 < str2);
        EXPECT_TRUE(str2 > str1);
    }

    {
        const StringAtom str1 = "AAA"_atom;

        EXPECT_TRUE(str1 == "AAA");
        EXPECT_TRUE("AAA" == str1);
        EXPECT_TRUE(str1 != "AAB");
        EXPECT_TRUE("AAB" != str1);

        EXPECT_TRUE(str1 < "AAB");
        EXPECT_TRUE("AAB" > str1);

        EXPECT_TRUE("AAA" >= str1);
        EXPECT_TRUE("AAA" <= str1);
        EXPECT_TRUE(str1 <= "AAA");
        EXPECT_TRUE(str1 >= "AAA");
    }

    {
        const StringAtom str1 = "AAA"_atom;

        EXPECT_TRUE(str1 == std::string("AAA"));
        EXPECT_TRUE(std::string("AAA") == str1);
        EXPECT_TRUE(str1 != std::string("AAB"));
        EXPECT_TRUE(std::string("AAB") != str1);

        EXPECT_TRUE(str1 < std::string("AAB"));
        EXPECT_TRUE(std::string("AAB") > str1);

        EXPECT_TRUE(std::string("AAA") >= str1);
        EXPECT_TRUE(std::string("AAA") <= str1);
        EXPECT_TRUE(str1 <= std::string("AAA"));
        EXPECT_TRUE(str1 >= std::string("AAA"));
    }


    const StringAtom str2 = StringAtom::Intern(std::string("AAA"));
    {
        const StringAtom str1 = "AAA"_atom;
        EXPECT_NE(str1, str2);

        EXPECT_TRUE(str1 < str2);
        EXPECT_TRUE(str2 > str1);
    }
}