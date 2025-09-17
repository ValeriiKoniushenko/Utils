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

#include "Core/StringHelper.h"

#include <gtest/gtest.h>
#include <sstream>

TEST(StringHelperTest, writeToStream)
{
    std::ostringstream captured;
    std::streambuf* old = std::cout.rdbuf(captured.rdbuf()); // redirect

    {
        auto string = "Test string, ignore it"_atom;
        std::cout << string << std::endl;
    }

    {
        const auto string = "Test string, ignore it"_atom;
        std::cout << string << std::endl;
    }

    {
        const auto string = "Test string, ignore it"_atom;
        std::stringstream ss;
        ss << string;
        EXPECT_EQ(ss.str(), "Test string, ignore it"_atom);
    }

    std::cout.rdbuf(old);
}

TEST(StringHelperTest, defaultInputStream)
{
    std::istringstream fakeInput("42 hello world\n");
    std::streambuf* oldCin = std::cin.rdbuf(fakeInput.rdbuf());  // redirect cin

    Core::StringAtom str;
    std::cin >> str >> str >> str;

    EXPECT_EQ(str, "42helloworld");
}

TEST(StringHelperTest, quickFormat)
{
    {
        Core::StringFormatter<char> formatter("Hello {}");
        formatter << "World";
        EXPECT_EQ("Hello World", formatter);
    }

    EXPECT_EQ("Hello World", "Hello {}"_f << "World");
    EXPECT_EQ("Hello 555", "Hello {}"_f << 555);
}
