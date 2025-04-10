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

#include "Core/Regex.h"

#include "Core/String.h"

#include <gtest/gtest.h>

using namespace Core;

TEST(RegexTest, SimpleRegex)
{
    const std::string subject = "1234 Hello# world!";

    RegexMatch regex;
    regex.setPattern("([A-Za-z]+)(#|!)");
    regex.setSubject(subject.c_str());
    regex.setCompileOptions(PCRE2_MULTILINE);

    ASSERT_TRUE(regex.compile());

    auto match = regex.match();

    ASSERT_TRUE(match);

    EXPECT_EQ("Hello#", subject.substr(match.offset, match.size));
}

TEST(RegexTest, ObjectCopying)
{
    const std::string subject = "1234 Hello# world!";

    RegexMatch regex;
    {
        RegexMatch re;
        re.setPattern("([A-Za-z]+)(#|!)");
        re.setSubject(subject.c_str());
        re.setCompileOptions(PCRE2_MULTILINE);
        regex = re;
    }

    ASSERT_TRUE(regex.compile());

    auto match = regex.match();

    ASSERT_TRUE(match);

    EXPECT_EQ("Hello#", subject.substr(match.offset, match.size));
}

TEST(RegexTest, ObjectMoving)
{
    const std::string subject = "1234 Hello# world!";

    RegexMatch regex;
    {
        RegexMatch re;
        re.setPattern("([A-Za-z]+)(#|!)");
        re.setSubject(subject.c_str());
        re.setCompileOptions(PCRE2_MULTILINE);
        regex = std::move(re);
    }

    ASSERT_TRUE(regex.compile());

    auto match = regex.match();

    ASSERT_TRUE(match);

    EXPECT_EQ("Hello#", subject.substr(match.offset, match.size));
}

TEST(RegexTest, SimpleCreateUsingOnlyPattern)
{
    const std::string subject = "1234 Hello# world!";

    RegexMatch regex("([A-Za-z]+)(#|!)");
    regex.setSubject(subject.c_str());
    regex.setCompileOptions(PCRE2_MULTILINE);

    ASSERT_TRUE(regex.compile());

    auto match = regex.match();

    ASSERT_TRUE(match);

    EXPECT_EQ("Hello#", subject.substr(match.offset, match.size));
}

TEST(RegexTest, SimpleCreate)
{
    RegexMatch regex("([A-Za-z]+)(#|!)", "1234 Hello# world!");
    regex.setCompileOptions(PCRE2_MULTILINE);

    ASSERT_TRUE(regex.compile());

    auto match = regex.match();

    ASSERT_TRUE(match);

    EXPECT_EQ(5, match.offset);
    EXPECT_EQ(6, match.size);
}

/*
TEST(RegexTest, DISABLED_WitoutCompile)
{
    RegexMatch regex("([A-Za-z]+)(#|!)", "1234 Hello# world!");
    regex.setCompileOptions(PCRE2_MULTILINE);

    auto match = regex.match();

    ASSERT_FALSE(match);
}
*/

TEST(RegexTest, MatchAll)
{
    RegexMatch regex("([A-Za-z]+)(#|!)", "1234 Hello# world!");
    regex.setCompileOptions(PCRE2_MULTILINE);
    regex.compile();

    auto match = regex.matchAll();

    ASSERT_FALSE(match.empty());
    ASSERT_EQ(match.size(), 2);

    EXPECT_EQ(5, match[0].offset);
    EXPECT_EQ(6, match[0].size);

    EXPECT_EQ(12, match[1].offset);
    EXPECT_EQ(6, match[1].size);
}

TEST(RegexTest, InvalidExpression)
{
    RegexMatch regex("(A-Za-z]+)(#|!)", "1234 Hello# world!");
    regex.compile();

    auto match = regex.matchAll();

    ASSERT_TRUE(match.empty());
}

TEST(RegexTest, InvalidExpression2)
{
    RegexMatch regex("(A-Za-z]+)(#|!)", "1234 Hello# world!");
    regex.compile();

    ASSERT_FALSE(regex.match());
}

TEST(RegexTest, IterateOverMatches)
{
    RegexMatch regex("[A-Za-z]+", "1234 Hello# world!");
    regex.compile();

    std::vector<RegexMatch::MatchedData> matches;
    regex.iterateOverMatches(
        [&matches](auto m)
        {
            matches.push_back(m);
        });

    ASSERT_FALSE(matches.empty());
    ASSERT_EQ(matches.size(), 2);

    EXPECT_EQ(5, matches[0].offset);
    EXPECT_EQ(5, matches[0].size);

    EXPECT_EQ(12, matches[1].offset);
    EXPECT_EQ(5, matches[1].size);
}

TEST(RegexTest, IterateOverMatchesOneTime)
{
    RegexMatch regex("[A-Za-z]+", "1234 Hello# world!");
    regex.compile();

    std::vector<RegexMatch::MatchedData> matches;
    regex.iterateOverMatches(
        [&matches](auto m) -> bool
        {
            matches.push_back(m);
            return false;
        });

    ASSERT_FALSE(matches.empty());
    ASSERT_EQ(matches.size(), 1);

    EXPECT_EQ(5, matches[0].offset);
    EXPECT_EQ(5, matches[0].size);
}

TEST(RegexTest, SimpleReplace)
{
    char buff[1024]{};

    RegexReplace regex("[0-9]+", "He3llo 123 world 456 how_are_you?");
    regex.setReplacementString("#");
    regex.setOutputString(buff, 1024);
    regex.setReplaceAll(true);
    regex.compile();

    ASSERT_TRUE(regex.replace());
    EXPECT_STREQ("He#llo # world # how_are_you?", buff);
}

TEST(RegexTest, MatchDataConverts)
{
    const std::string subject = "1234 Hello# world!";
    const std::string_view view = subject;

    RegexMatch::MatchedData match{ 5, 6 };

    {
        const auto stdString = match.convertBasedOn(subject);
        EXPECT_EQ("Hello#", stdString);
    }

    {
        const auto stdString = match.convertBasedOn<std::string>(view.data(), view.size());
        EXPECT_EQ("Hello#", stdString);
    }

    {
        const auto vector = match.convertBasedOn<std::vector<char>>(view.data(), view.size());
        ASSERT_EQ(vector.size(), 6);
        EXPECT_EQ(0, memcmp("Hello#", vector.data(), 6));
    }

    {
        const auto atom = match.convertBasedOn<StringAtom>(view.data(), view.size());
        EXPECT_EQ("Hello#", atom);
    }
}
