// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "Core/String.h"

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
    std::streambuf* oldCin = std::cin.rdbuf(fakeInput.rdbuf()); // redirect cin

    Core::StringAtom str;
    std::cin >> str >> str >> str;

    EXPECT_EQ(str, "42helloworld");
}

TEST(StringHelperTest, inputOperatorUsesTheProvidedStream)
{
    std::istringstream input("42 hello");
    Core::StringAtom string;

    input >> string >> string;

    EXPECT_EQ("42hello", string);
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

TEST(StringHelperTest, InternedStringBecomesDynamicOnMutation)
{
    // tag::atomic_string_mutation[]
    Core::StringAtom string = "hello"_atom;
    ASSERT_TRUE(string.isStatic());

    string.pushBack("!");

    ASSERT_FALSE(string.isStatic());
    EXPECT_EQ("hello!", string);
    // end::atomic_string_mutation[]
}

TEST(StringHelperTest, NarrowStringSupportsRegexMatching)
{
    // tag::string_regex_match[]
    auto string = Core::StringAtom::Intern("RegEx");

    EXPECT_TRUE(string.regexMatch("^([A-Z][a-z0-9]+)+$"));
    // end::string_regex_match[]
}
