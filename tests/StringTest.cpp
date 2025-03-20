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

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <unordered_set>

TEST(StringTest, BaseString_char_Assigning)
{
    using Core::StringAtom;

    {
        StringAtom string;
        string = "Hello";
        EXPECT_EQ(string, "Hello");
    }

    {
        StringAtom string;
        std::string s = "Hello";
        string = s;
        EXPECT_EQ(string, "Hello");
    }

    {
        StringAtom string;
        string = std::string("Hello");
        EXPECT_EQ(string, "Hello");
    }

    {
        StringAtom string;
        std::string_view s = "Hello";
        string = s;
        EXPECT_EQ(string, "Hello");
    }

    {
        StringAtom string;
        string = std::string_view("Hello");
        EXPECT_EQ(string, "Hello");
    }

    {
        StringAtom string;
        StringAtom s = "Hello";
        string = s;
        EXPECT_EQ(string, "Hello");
    }

    {
        StringAtom string;
        string = StringAtom("Hello");
        EXPECT_EQ(string, "Hello");
    }

    EXPECT_EQ("Hello", StringAtom("Hello"));
    EXPECT_EQ("Hello", StringAtom(std::string("Hello")));
    EXPECT_EQ("Hello", StringAtom(std::string_view("Hello")));
}

TEST(StringTest, BaseString_char_default__Creation)
{
    using Core::StringAtom;

    {
        const StringAtom str1 = "";

        EXPECT_EQ("", str1);
        EXPECT_EQ(0, str1.size());
        EXPECT_NE(0, str1.capacity());
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

TEST(StringTest, BaseString_char_default__DefaultCopyAndMove)
{
    using Core::StringAtom;

    {
        StringAtom str1 = "Hello"_atom;
        StringAtom str2(str1);

        ASSERT_FALSE(str1.isEmpty());
        ASSERT_FALSE(str2.isEmpty());
        EXPECT_EQ(str1.c_str(), str2.c_str());
        EXPECT_TRUE(str1.isStatic());
        EXPECT_TRUE(str2.isStatic());
        EXPECT_EQ(5, str1.size());
        EXPECT_EQ(5, str2.size());
        EXPECT_EQ("Hello", str1);
        EXPECT_EQ("Hello", str2);
    }

    {
        StringAtom str1 = "Hello"_atom;
        StringAtom str2(std::move(str1));

        ASSERT_TRUE(str1.isEmpty());
        ASSERT_FALSE(str2.isEmpty());
        EXPECT_NE(str1.c_str(), str2.c_str());
        EXPECT_FALSE(str1.isStatic());
        EXPECT_TRUE(str2.isStatic());
        EXPECT_EQ(0, str1.size());
        EXPECT_EQ(5, str2.size());
        EXPECT_EQ("Hello", str2);
    }

    {
        StringAtom str1 = "Hello";
        StringAtom str2(str1);

        ASSERT_FALSE(str1.isEmpty());
        ASSERT_FALSE(str2.isEmpty());
        EXPECT_NE(str1.c_str(), str2.c_str());
        EXPECT_TRUE(str1.isDynamic());
        EXPECT_TRUE(str2.isDynamic());
        EXPECT_EQ(5, str1.size());
        EXPECT_EQ(5, str2.size());
        EXPECT_EQ("Hello", str1);
        EXPECT_EQ("Hello", str2);
    }

    {
        StringAtom str1 = "Hello";
        StringAtom str2(std::move(str1));

        ASSERT_TRUE(str1.isEmpty());
        ASSERT_FALSE(str2.isEmpty());
        EXPECT_NE(str1.c_str(), str2.c_str());
        EXPECT_EQ(nullptr, str1.c_str());
        EXPECT_FALSE(str1.isDynamic());
        EXPECT_TRUE(str2.isDynamic());
        EXPECT_EQ(0, str1.size());
        EXPECT_EQ(5, str2.size());
        EXPECT_EQ("Hello", str2);
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

    const StringAtom str2 = StringAtom::Intern(std::string("AAB"));
    {
        const StringAtom str1 = "AAA"_atom;
        EXPECT_NE(str1, str2);

        EXPECT_TRUE(str1 < str2);
        EXPECT_TRUE(str2 > str1);
    }

    {
        EXPECT_EQ("Hello"_atom, "Hello"_atom);
        EXPECT_TRUE("Hello"_atom == "Hello"_atom);
        EXPECT_TRUE("Hello"_atom == std::string("Hello"));
        EXPECT_TRUE("Hello"_atom == std::string_view("Hello").data());
    }

    {
        EXPECT_TRUE("Hello"_atom < "Hello1");
        EXPECT_FALSE("Hello"_atom > "Hello1");
        EXPECT_TRUE("Hello"_atom >= "Hello");
        EXPECT_TRUE("Hello"_atom <= "Hello");
        EXPECT_TRUE("Hello"_atom <= "Hello1");
        EXPECT_TRUE("Hello1"_atom >= "Hello");

        EXPECT_TRUE("Hello"_atom < "Hello1"_atom);
        EXPECT_FALSE("Hello"_atom > "Hello1"_atom);
        EXPECT_TRUE("Hello"_atom != "Hello1"_atom);
        EXPECT_TRUE("Hello"_atom <= "Hello1"_atom);
        EXPECT_TRUE("Hello1"_atom >= "Hello"_atom);
        EXPECT_TRUE("Hello"_atom >= "Hello"_atom);
        EXPECT_TRUE("Hello"_atom <= "Hello"_atom);

        EXPECT_TRUE("Hello"_atom != std::string("Hello1"));
        EXPECT_TRUE("Hello"_atom >= std::string("Hello"));
        EXPECT_TRUE("Hello"_atom <= std::string("Hello"));
        EXPECT_TRUE("Hello1"_atom >= std::string("Hello"));
        EXPECT_TRUE("Hello"_atom <= std::string("Hello1"));

        EXPECT_TRUE("Hello"_atom != std::string_view("Hello1").data());
        EXPECT_TRUE("Hello"_atom >= std::string_view("Hello").data());
        EXPECT_TRUE("Hello"_atom <= std::string_view("Hello").data());
        EXPECT_TRUE("Hello1"_atom >= std::string("Hello").data());
        EXPECT_TRUE("Hello"_atom <= std::string_view("Hello1").data());
    }
}

TEST(StringTest, BaseString_char_OperationsWithEmptyString)
{
    using Core::StringAtom;

    {
        StringAtom str;
        auto dynamic = str.getCopyAsDynamic();
        EXPECT_EQ(0, dynamic.size());
        EXPECT_EQ(0, dynamic.capacity());
        EXPECT_EQ(nullptr, dynamic.c_str());
    }

    {
        StringAtom str;

        EXPECT_EQ(0, str.makeHash());
        EXPECT_EQ(str.end(), str.begin());
        EXPECT_TRUE("" == str);
        EXPECT_TRUE(str == "");
    }

    {
        StringAtom str = "";

        EXPECT_EQ(0, str.makeHash());
        EXPECT_EQ(str.end(), str.begin());
        EXPECT_TRUE("" == str);
        EXPECT_TRUE(str == "");
    }

    {
        StringAtom str;
        str.push_back("Hello");
        EXPECT_EQ(5, str.size());
        EXPECT_GE(str.capacity(), 5);
    }

    {
        StringAtom str;
        str.push_front("Hello");
        EXPECT_EQ(5, str.size());
        EXPECT_GE(str.capacity(), 5);
    }

    {
        StringAtom str;
        EXPECT_FALSE(str.regexFind("Hello").isMatched());
    }

    {
        StringAtom str;
        EXPECT_TRUE(str.regexFindAll("Hello").empty());
    }

    {
        StringAtom str;
        EXPECT_FALSE(str.regexMatch("Hello"));

        str.shrink_to_fit();
    }
}

TEST(StringTest, BaseString_char_default__InStdSet)
{
    using Core::StringAtom;

    {
        std::set<StringAtom> set;
        set.emplace(StringAtom::Intern("Hello"));
        set.emplace(StringAtom::Intern("World"));
        set.emplace(StringAtom::Intern("Hello"));

        EXPECT_TRUE(set.size() == 2);

        auto itHello = set.find("Hello"_atom);
        ASSERT_TRUE(itHello != set.end());
        EXPECT_EQ(*itHello, "Hello");
        EXPECT_EQ(*itHello, "Hello"_atom);

        set.emplace("AAA"_atom);
        EXPECT_TRUE(set.size() == 3);
        auto itAaa = set.find("AAA"_atom);
        ASSERT_TRUE(itAaa != set.end());
        EXPECT_EQ(*itAaa, "AAA");
        EXPECT_EQ(*itAaa, "AAA"_atom);
    }
}

TEST(StringTest, BaseString_char_default__InStdUnorderedSet)
{
    using Core::StringAtom;

    {
        std::unordered_set<StringAtom> set;
        set.emplace(StringAtom::Intern("Hello"));
        set.emplace(StringAtom::Intern("World"));
        set.emplace(StringAtom::Intern("Hello"));

        EXPECT_TRUE(set.size() == 2);

        auto itHello = set.find("Hello"_atom);
        ASSERT_TRUE(itHello != set.end());
        EXPECT_EQ(*itHello, "Hello");
        EXPECT_EQ(*itHello, "Hello"_atom);

        set.emplace("AAA"_atom);
        EXPECT_TRUE(set.size() == 3);
        auto itAaa = set.find("AAA"_atom);
        ASSERT_TRUE(itAaa != set.end());
        EXPECT_EQ(*itAaa, "AAA");
        EXPECT_EQ(*itAaa, "AAA"_atom);
    }
}

TEST(StringTest, BaseString_char_default__Converts)
{
    using Core::StringAtom;

    {
        const StringAtom str = "123"_atom;
        EXPECT_EQ(123, str.convertTo<int>());
    }

    {
        const StringAtom str = "123.1234"_atom;
        EXPECT_EQ(123.1234f, str.convertTo<float>());
    }

    {
        const StringAtom str = "1231234567"_atom;
        EXPECT_EQ(1231234567, str.convertTo<long long>());
    }

    {
        const StringAtom str = "f1231234567"_atom;
        EXPECT_EQ(0, str.convertTo<long long>());
    }

    {
        const StringAtom str = "1231234567f"_atom;
        EXPECT_EQ(1231234567, str.convertTo<long long>());
    }
}

TEST(StringTest, BaseString_char_default__UtilsFunctions)
{
    using Core::StringAtom;

    // Split
    {
        const StringAtom str = "Hello fucking world!"_atom;
        auto tokens = str.split(" ");
        ASSERT_EQ(3, tokens.size());
        EXPECT_EQ(tokens[0], "Hello");
        EXPECT_EQ(tokens[1], "fucking");
        EXPECT_EQ(tokens[2], "world!");
    }
}

TEST(StringTest, BaseString_char_default__Iterator)
{
    using Core::StringAtom;

    {
        StringAtom str = "Hello world!"_atom;
        auto i = str.begin();
        EXPECT_EQ('H', *i);
    }

    {
        StringAtom str = "Hello world!"_atom;
        auto i = str.cbegin();
        EXPECT_EQ('H', *i);
    }

    {
        StringAtom str = "Hello world!"_atom;
        auto ci = str.cbegin() + 1;
        auto i = str.begin() + 1;
        EXPECT_EQ('e', *ci);
        EXPECT_EQ('e', *i);
    }

    {
        StringAtom str = "Hello world!"_atom;
        auto i = str.end() - 1;
        EXPECT_EQ('!', *i);
    }

    {
        StringAtom str = "Hello world!"_atom;
        auto i = str.rbegin() + 1;
        EXPECT_EQ('!', *i);
    }

    {
        StringAtom str = "Hello world!"_atom;
        auto front = str.front();
        EXPECT_EQ('H', front);

        auto back = str.back();
        EXPECT_EQ('!', back);
    }

    {
        StringAtom str = "Hello world!"_atom;
        auto begin = str.begin();
        auto end = str.end();

        EXPECT_TRUE(begin != end);
        EXPECT_FALSE(begin == end);
    }

    {
        StringAtom str = "Hello world!"_atom;
        std::string buff;

        for (auto ch : str)
        {
            buff.push_back(ch);
        }
        EXPECT_EQ(str, buff);
    }

    {
        std::ofstream out("temp.txt");
        ASSERT_TRUE(out.is_open());
        out << "Hello world!";
        out.close();

        std::ifstream in("temp.txt");
        ASSERT_TRUE(in.is_open());

        StringAtom str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        ASSERT_FALSE(str.isEmpty());
        EXPECT_TRUE(str.isDynamic());
        EXPECT_EQ("Hello world!", str);
        EXPECT_EQ(12, str.size());
        in.close();
        std::filesystem::remove("temp.txt");
    }
}

TEST(StringTest, BaseString_char_default_Modifications_RangeBasedFor)
{
    using Core::StringAtom;

    {
        StringAtom str = "Hello world!"_atom;
        std::string tempStdStr;
        for (auto ch : str)
        {
            tempStdStr.push_back(ch);
        }

        EXPECT_EQ(tempStdStr, str);
    }
}

TEST(StringTest, BaseString_char_default_Modifications_SubStr)
{
    using Core::StringAtom;

    {
        StringAtom str = "Hello world!"_atom;
        str.subStr(6);
        EXPECT_EQ("world!", str);
        EXPECT_EQ(6, str.size());
    }

    {
        StringAtom str = "Hello world!"_atom;
        str.subStr(0, 5);
        EXPECT_EQ("Hello", str);
        EXPECT_EQ(5, str.size());
    }

    {
        StringAtom str = "Hello world!"_atom;
        str.subStr(2, 5);
        EXPECT_EQ("llo", str);
        EXPECT_EQ(3, str.size());
    }
}

TEST(StringTest, BaseString_char_default_Find)
{
    using Core::StringAtom;

    {
        const auto str = "Hello world!"_atom;
        const auto* found = str.find(" ");
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = "Hello world!"_atom;
        const auto* found = str.find(std::string(" "));
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = "Hello world!"_atom;
        const auto* found = str.find(" "_atom);
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = "Hello world! How are you, world?"_atom;
        const auto strings = str.findAll("world");
        ASSERT_FALSE(strings.empty());
        for (const auto& string : strings)
        {
            EXPECT_EQ("world", std::string_view(string, 5));
        }
    }
}

TEST(StringTest, BaseString_char_default_Cmp)
{
    using Core::StringAtom;

    {
        const auto str = "Hello world!"_atom;
        EXPECT_TRUE(str.compare("hello world!", true) == Core::Comparison::Equal);
        EXPECT_TRUE(str.compare("hello world", true) == Core::Comparison::Less);
    }

    {
        const auto str = "AAA"_atom;
        EXPECT_TRUE(str.compare("bbb", true) == Core::Comparison::Less);
    }
}

TEST(StringTest, BaseString_char_default_Trim)
{
    using Core::StringAtom;

    {
        auto str = "  MyLogin"_atom;
        str.trimStart(' ');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = "__MyLogin"_atom;
        str.trimStart(' ');
        EXPECT_EQ("__MyLogin", str);
        EXPECT_EQ(9, str.size());
    }

    {
        auto str = "MyLogin  "_atom;
        str.trimEnd(' ');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = "MyLogin__"_atom;
        str.trimEnd(' ');
        EXPECT_EQ("MyLogin__", str);
        EXPECT_EQ(9, str.size());
    }

    {
        auto str = "MyLogin  "_atom;
        str.trim(' ');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = "  MyLogin  "_atom;
        str.trim(' ');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = "MyLogin__"_atom;
        str.trim(' ');
        EXPECT_EQ("MyLogin__", str);
        EXPECT_EQ(9, str.size());
    }

    {
        auto str = "MyLogin;"_atom;
        str.trim(';');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = "   "_atom;
        str.trim(' ');
        EXPECT_EQ("", str);
        EXPECT_EQ(0, str.size());
        EXPECT_NE(0, str.capacity());
    }

    {
        auto str = "   "_atom;
        str.trimEnd(' ');
        EXPECT_EQ("", str);
        EXPECT_EQ(0, str.size());
        EXPECT_NE(0, str.capacity());
    }
}

TEST(StringTest, BaseString_char_default_ToLower)
{
    {
        auto str = "Hello World!"_atom;
        str.toLowerCase();
        EXPECT_EQ("hello world!", str);
        EXPECT_EQ(12, str.size());
    }
}

TEST(StringTest, BaseString_char_default_ToUpper)
{
    {
        auto str = "Hello World!"_atom;
        str.toUpperCase();
        EXPECT_EQ("HELLO WORLD!", str);
        EXPECT_EQ(12, str.size());
    }
}

TEST(StringTest, BaseString_char_default_PushBack)
{
    {
        auto str = "Hello World"_atom;
        str.push_back('!');
        EXPECT_EQ("Hello World!", str);
        EXPECT_EQ(12, str.size());
    }

    {
        auto str = "Hello World"_atom;
        const auto* text =
            R"(Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum)";
        str.push_back(text);
        EXPECT_EQ(
            R"(Hello WorldLorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum)",
            str);
        EXPECT_EQ(11 + strlen(text), str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.push_back(std::string("!!!"));
        EXPECT_EQ("Hello World!!!", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.push_back(std::string("!!!"));
        EXPECT_EQ("Hello World!!!", str);
        EXPECT_EQ(14, str.size());
    }
}

TEST(StringTest, BaseString_char_default_PushFront)
{
    {
        auto str = "Hello World"_atom;
        str.push_front('!');
        EXPECT_EQ("!Hello World", str);
        EXPECT_EQ(12, str.size());
    }

    {
        auto str = "Hello World"_atom;
        const auto* text =
            R"(Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum)";
        str.push_front(text);
        EXPECT_EQ(
            R"(Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem IpsumHello World)",
            str);
        EXPECT_EQ(11 + strlen(text), str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.push_front(std::string("!!!"));
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.push_front(std::string("!!!"));
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }
}

TEST(StringTest, BaseString_char_default_Insert)
{
    {
        auto str = "Hello World"_atom;
        str.insert(0, "!!!");
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(0, "!!!");
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(str.size(), "!!!");
        EXPECT_EQ("Hello World!!!", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(str.size(), "!!!");
        EXPECT_EQ("Hello World!!!", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(5, "!!!");
        EXPECT_EQ("Hello!!! World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(5, "!!!");
        EXPECT_EQ("Hello!!! World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(str.begin(), "!!!");
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(str.begin(), "!!!");
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }
}

TEST(StringTest, BaseString_char_default_PopBack)
{
    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.size());

        str.pop_back();

        EXPECT_EQ("Hello World", str);
        EXPECT_EQ(11, str.size());
    }

    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.size());

        str.pop_back();

        EXPECT_EQ("Hello World", str);
        EXPECT_EQ(11, str.size());
    }
}

TEST(StringTest, BaseString_char_default_PopFront)
{
    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.size());
        std::string s;
        str.pop_front();

        EXPECT_EQ("ello World!", str);
        EXPECT_EQ(11, str.size());
    }

    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.size());
        std::string s;
        str.pop_front();

        EXPECT_EQ("ello World!", str);
        EXPECT_EQ(11, str.size());
    }
}

TEST(StringTest, BaseString_char_default_ShrinkToFit)
{
    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(12 + 1, str.capacity());

        str.reserve(100);
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(201, str.capacity());

        str.shrink_to_fit();
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(12 + 1, str.capacity());
    }

    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(12 + 1, str.capacity());

        str.reserve(100);
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(201, str.capacity());

        str.shrink_to_fit();
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(12 + 1, str.capacity());
    }
}

TEST(StringTest, BaseString_char_default_Replace)
{
    {
        auto str = "Hello World! Hello World!"_atom;
        str.replaceFirst("llo", "LLO___LLO");
        EXPECT_EQ("HeLLO___LLO World! Hello World!", str);
    }

    {
        auto str = "Hello World! Hello World!"_atom;
        str.replaceAll("o", "!o!");
        EXPECT_EQ("Hell!o! W!o!rld! Hell!o! W!o!rld!", str);
    }
}

TEST(StringTest, BaseString_char_default_Copy)
{
    {
        const auto str = "Hello world!"_atom;
        char arr[128]{};
        str.copyTo(arr, str.size());
        EXPECT_EQ(str, arr);
    }
}

TEST(StringTest, BaseString_char_AdvanceWorkFlow)
{
    auto str = "Hello World!"_atom;

    {
        auto tokens = str.split(" ");
        ASSERT_EQ(2, tokens.size());
        EXPECT_EQ("Hello", tokens[0]);
        EXPECT_EQ("World!", tokens[1]);
    }

    str.toLowerCase();
    ASSERT_EQ("hello world!", str);
    ASSERT_EQ(str, "hello world!");
    ASSERT_EQ(std::string("hello world!"), str);
    ASSERT_EQ(str, std::string("hello world!"));

    str.toUpperCase();
    ASSERT_EQ("HELLO WORLD!", str);
    ASSERT_EQ(str, "HELLO WORLD!");
    ASSERT_EQ(std::string("HELLO WORLD!"), str);
    ASSERT_EQ(str, std::string("HELLO WORLD!"));

    str.replaceAll("!", "???");
    ASSERT_EQ("HELLO WORLD???", str);
    ASSERT_EQ(str, "HELLO WORLD???");
    ASSERT_EQ(std::string("HELLO WORLD???"), str);
    ASSERT_EQ(str, std::string("HELLO WORLD???"));
    ASSERT_TRUE(strcmp(str.c_str(), "HELLO WORLD???") == 0);

    ASSERT_TRUE(str.compare("AAA", true) == Core::Comparison::Greater);
    ASSERT_TRUE(str.compare("AAA") == Core::Comparison::Greater);
    ASSERT_TRUE(str.compare("aaa") == Core::Comparison::Less);

    str.trim('?');
    ASSERT_EQ(str, "HELLO WORLD");
}

TEST(StringTest, BaseString_char_AdvanceWorkFlow2)
{
    Core::StringAtom str(128);
    ASSERT_TRUE(str.isEmpty());
    ASSERT_TRUE(!str);
    if (!str)
    {
        str.push_back("Hello");
        ASSERT_FALSE(str.isEmpty());
        str.push_back("World");
        ASSERT_FALSE(str.isEmpty());
        ASSERT_EQ("HelloWorld", str);
    }
}

TEST(StringTest, BaseString_char_AdvanceWorkFlow3)
{
    Core::StringAtom str;
    str.resize(5);
    if (!str)
    {
        str.push_back("Hello");
        ASSERT_FALSE(str.isEmpty());
        str.push_back("World");
        ASSERT_FALSE(str.isEmpty());
        ASSERT_EQ("     HelloWorld", str);
    }
}

TEST(StringTest, BaseString_char_default__From)
{
    using Core::StringAtom;
    EXPECT_EQ("123", StringAtom::MakeFrom(123));
    EXPECT_EQ("123.000000", StringAtom::MakeFrom(123.f));
    EXPECT_EQ("123.000000", StringAtom::MakeFrom(123.));
    EXPECT_EQ("412312334234", StringAtom::MakeFrom(412312334234ull));
}

TEST(StringTest, BaseString_char_default__Format)
{
    using Core::StringAtom;
    const auto str = StringAtom::Format("Hello {}! I have {}$. If u want we can go to {}.", "Jenny", 300, "caffee");
    EXPECT_EQ("Hello Jenny! I have 300$. If u want we can go to caffee.", str);
}

TEST(StringTest, BaseString_char_default__LinesCount)
{
    using Core::StringAtom;
    const auto str = R"(Hello
World!
How are you?)"_atom;
    EXPECT_EQ(3, StringAtom::GetLinesCountInText(str.c_str(), str.c_str() + str.size()));
}

TEST(StringTest, BaseString_char_default__iterate_over_lines)
{
    using Core::StringAtom;
    const auto str = "Hello\nWorld!\nHow are you?"_atom;

    std::vector<StringAtom> lines = { "Hello", "World!", "How are you?" };

    std::vector<bool> linesValidator;
    int i = 0;
    str.forEachByLine(
        [&](auto str)
        {
            linesValidator.push_back(str == lines[i++]);
            return true;
        },
        StringAtom::LineSeparator::LF);

    ASSERT_EQ(lines.size(), linesValidator.size());

    for (auto isValid : linesValidator)
    {
        EXPECT_TRUE(isValid);
    }
}

TEST(StringTest, BaseString_char_default__Erase)
{
    using Core::StringAtom;
    {
        auto str = "Hello world!"_atom;
        str.erase(5);
        EXPECT_EQ("Helloworld!", str);
    }

    {
        auto str = "Hello world!"_atom;
        str.erase(5, 7);
        EXPECT_EQ("Hellorld!", str);
    }

    {
        auto str = "Hello world!"_atom;
        str.erase(str.begin() + 5);
        EXPECT_EQ("Helloworld!", str);
    }

    {
        auto str = "Hello world!"_atom;
        str.erase(str.begin() + 5, str.begin() + 7);
        EXPECT_EQ("Hellorld!", str);
    }
}

TEST(StringTest, BaseString_char_default__Misc)
{
    EXPECT_TRUE(Core::StringAtom::IsSpace(' '));
    EXPECT_TRUE(Core::StringAtom::IsSpace('\n'));
    EXPECT_TRUE(Core::StringAtom::IsSpace('\t'));
    EXPECT_TRUE(Core::StringAtom::IsSpace('\r'));
    EXPECT_FALSE(Core::StringAtom::IsSpace('a'));

    EXPECT_TRUE(Core::StringAtom::IsContainChar('a', "abcdef"));
    EXPECT_TRUE(Core::StringAtom::IsContainChar('c', "abcdef"));
    EXPECT_TRUE(Core::StringAtom::IsContainChar('f', "abcdef"));
    EXPECT_FALSE(Core::StringAtom::IsContainChar('z', "abcdef"));
}

TEST(StringTest, BaseString_char_addition)
{
    EXPECT_EQ("Hello world!", "Hello " + Core::StringAtom("world!"));
    EXPECT_EQ("Hello world!", "Hello " + "world!"_atom);
    EXPECT_EQ("Hello world!", "Hello " + "world"_atom + "!");
    EXPECT_EQ("Hello world!", "Hello " + "world"_atom + "!"_atom);
    EXPECT_EQ("Hello world!", Core::StringAtom("Hello ") + "world!");
}

TEST(StringTest, BaseString_char_simple_copy)
{
    Core::StringAtom str = "Hello world!";
    const auto baseCapacity = str.capacity();
    str.shrink_to_fit();
    const auto shrinkedCapacity = str.capacity();
    EXPECT_LE(shrinkedCapacity, baseCapacity);

    Core::StringAtom copyStr = "Smth";
    copyStr = str;
    EXPECT_EQ(shrinkedCapacity, str.capacity());
}

TEST(StringTest, BaseString_char_working_with_std_filesystem_path)
{
    {
        auto str = "Path: "_dyn;
        str += Core::StringAtom::MakeFrom(std::filesystem::current_path());
    }

    {
        auto str = "Path: "_dyn;
        auto path = std::filesystem::current_path();
        str += Core::StringAtom::MakeFrom(path);
    }

    {
        auto str = "Path: "_dyn;
        const auto path = std::filesystem::current_path();
        str += Core::StringAtom::MakeFrom(path);
    }

    {
        auto path = std::filesystem::current_path();
        auto str = "Path: "_dyn + Core::StringAtom::MakeFrom(path);
    }

    {
        auto str = "Path: "_dyn + Core::StringAtom::MakeFrom(std::filesystem::current_path());
    }
}

TEST(StringTest, BaseString_char_foreachbyline)
{
    using namespace Core;

    StringAtom str = "Hello\nWorld\n!";
    std::vector<StringAtom> tokens;
    str.forEachByLine(
        [&tokens](auto str)
        {
            tokens.emplace_back(std::move(str));
        });

    ASSERT_EQ(3, tokens.size());
    EXPECT_EQ("Hello", tokens[0]);
    EXPECT_EQ("World", tokens[1]);
    EXPECT_EQ("!", tokens[2]);
}

TEST(StringTest, BaseString_char_FindNextLine)
{
    using namespace Core;

    StringAtom str = "Hello\nWorld\n!";
    std::vector<StringAtom> tokens = { "Hello", "World", "!" };

    const auto* ptr = str.c_str();

    const auto firstLineStr = StringAtom(ptr, tokens[0].size());
    ASSERT_TRUE(firstLineStr);
    ASSERT_EQ("Hello", firstLineStr);

    const auto secondLine = StringAtom::FindNextLine(ptr);
    ASSERT_TRUE(secondLine);
    const auto secondLineStr = StringAtom(secondLine, tokens[1].size());
    ASSERT_EQ("World", secondLineStr);
    ptr = secondLine;

    const auto thirdLine = StringAtom::FindNextLine(ptr);
    ASSERT_TRUE(thirdLine);
    const auto thirdLineStr = StringAtom(thirdLine, tokens[2].size());
    ASSERT_EQ("!", thirdLineStr);
    ptr = thirdLine;

    EXPECT_EQ(nullptr, StringAtom::FindNextLine(ptr));
}

TEST(StringTest, BaseString_char_ReverseStrStr)
{
    using namespace Core;

    const auto lenHello = StringAtom("Hello").length();

    {
        StringAtom str = "HelloHello";
        EXPECT_EQ(str.c_str() + lenHello, str.reverseFind("Hello"));
    }

    {
        StringAtom str = "HelloHello";
        EXPECT_EQ(str.c_str(), str.reverseFind("Hello", 0, 1));
    }

    {
        StringAtom str = "";
        EXPECT_EQ(nullptr, str.reverseFind("Hello", 0, 1));
    }
}

TEST(StringTest, BaseString_char_FindPrevLine)
{
    using namespace Core;

    StringAtom str = "Hello\nWorld\n!";
    std::vector<StringAtom> tokens = { "Hello", "World", "!" };

    const auto* end = str.c_str() + str.size();

    const auto* ptr = StringAtom::FindPrevLine(str.c_str());
    ASSERT_TRUE(ptr);
    const auto thirdLine = StringAtom(ptr, tokens[2].size());
    EXPECT_EQ("!", thirdLine);

    ptr = StringAtom::FindPrevLine(str.c_str(), ptr - 1);
    ASSERT_TRUE(ptr);
    const auto secondLine = StringAtom(ptr, tokens[1].size());
    EXPECT_EQ("World", secondLine);

    ptr = StringAtom::FindPrevLine(str.c_str(), ptr - 1);
    ASSERT_TRUE(ptr);
    const auto firstLine = StringAtom(ptr, tokens[0].size());
    EXPECT_EQ("Hello", firstLine);
}

#ifdef UTILS__USE_STD_REGEX

TEST(StringTest, BaseString_char_default_Regex)
{
    {
        // check for pascal case
        auto str = "RegEx"_atom;
        EXPECT_TRUE(str.RegexMatch("^([A-Z][a-z0-9]+)+$"));
    }

    {
        auto str = "RegEx"_atom;
        Core::StringAtom::StdRegexMatchResults match;
        EXPECT_TRUE(str.RegexMatch("^([A-Z][a-z0-9]+)+$", match));
        EXPECT_FALSE(match.empty());
    }

    {
        auto str = "Hello this fucking world!"_atom;
        str.RegexReplace(" ", "_=_");
        EXPECT_EQ("Hello_=_this_=_fucking_=_world!", str);
    }
}

TEST(StringTest, BaseString_char_RegexFind)
{
    using Core::StringAtom;

    {
        StringAtom str = "Hello world!"_atom;
        const auto match = str.RegexFind(" \\w+");
        EXPECT_EQ(" world", match.str());
    }

    {
        StringAtom str = "Hello world!"_atom;
        const auto match = str.RegexFind(" \\w+", 3);
        EXPECT_EQ(" world", match.str());
    }

    {
        StringAtom str = "Hello world!";
        const auto match = str.RegexFind(" \\w+");
        EXPECT_EQ(" world", match.str());
    }

    {
        StringAtom str = "Hello world!";
        const auto match = str.RegexFind(" \\w+", 3);
        EXPECT_EQ(" world", match.str());
    }

    {
        StringAtom str = "Hello world!";
        const auto match = str.RegexFind(" \\w+");
        EXPECT_EQ(" world", match.str());
    }

    {
        StringAtom str = "Hello world!";
        const auto match = str.RegexFind(" \\w+", 3);
        EXPECT_EQ(" world", match.str());
    }
}

TEST(StringTest, BaseString_char_RegexIterate)
{
    using Core::StringAtom;

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        str.RegexIterate("\\w+",
                         [&buffer](const StringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        str.RegexIterate(std::string("\\w+"),
                         [&buffer](const StringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        std::string expr("\\w+");
        str.RegexIterate(expr,
                         [&buffer](const StringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        std::string_view expr("\\w+");
        str.RegexIterate(expr,
                         [&buffer](const StringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        const auto expr = "\\w+"_atom;
        str.RegexIterate(expr,
                         [&buffer](const StringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        str.RegexIterate("\\w+"_atom,
                         [&buffer](const StringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        str.RegexIterate(std::string_view("\\w+"),
                         [&buffer](const StringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }
}

TEST(StringTest, BaseString_char_default__RegexReplace)
{
    auto str = "Hello world!"_atom;
    EXPECT_FALSE(str.RegexReplace("\\?", ""));
    EXPECT_EQ("Hello world!", str);
    EXPECT_TRUE(str.RegexReplace(" ", "_"));
    EXPECT_EQ("Hello_world!", str);
}

#else

TEST(StringTest, BaseString_char_RegexMatch)
{
    {
        // check for pascal case
        auto str = "RegEx"_atom;
        EXPECT_TRUE(str.regexMatch("^([A-Z][a-z0-9]+)+$"));
    }

    {
        auto str = "RegEx\n\rHello"_atom;
        EXPECT_TRUE(str.regexMatch("^(\\w+\\s*)+$"));
    }
}

TEST(StringTest, BaseString_char_RegexFind)
{
    using Core::StringAtom;

    {
        StringAtom str = "Hello world!"_atom;
        const auto match = str.regexFind(" \\w+");
        ASSERT_TRUE(match.isMatched());
        EXPECT_EQ(" world", match.convertBasedOn(str));
    }

    {
        StringAtom str = "Hello world!"_atom;
        const auto match = str.regexFind(" \\w+", 3);
        ASSERT_TRUE(match.isMatched());
        EXPECT_EQ(" world", match.convertBasedOn(str));
    }

    {
        StringAtom str = "Hello world!";
        const auto match = str.regexFind(" \\w+");
        ASSERT_TRUE(match.isMatched());
        EXPECT_EQ(" world", match.convertBasedOn(str));
    }

    {
        StringAtom str = "Hello world!";
        const auto match = str.regexFind(" \\w+", 3);
        ASSERT_TRUE(match.isMatched());
        EXPECT_EQ(" world", match.convertBasedOn(str));
    }

    {
        StringAtom str = "Hello world!";
        const auto match = str.regexFind(" \\w+");
        EXPECT_EQ(" world", match.convertBasedOn(str));
    }

    {
        StringAtom str = "Hello world!";
        const auto match = str.regexFind(" \\w+", 3);
        EXPECT_EQ(" world", match.convertBasedOn(str));
    }
}

TEST(StringTest, BaseString_char_RegexFindAll)
{
    auto str = "How are you, Jim?"_atom;
    auto vec = str.regexFindAll("\\w+");
    ASSERT_EQ(4, vec.size());
    EXPECT_EQ("How", vec[0].convertBasedOn(str));
    EXPECT_EQ("are", vec[1].convertBasedOn(str));
    EXPECT_EQ("you", vec[2].convertBasedOn(str));
    EXPECT_EQ("Jim", vec[3].convertBasedOn(str));
}

TEST(StringTest, BaseString_char_RegexIterate)
{
    using Core::StringAtom;

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        str.regexIterate("\\w+",
                         [&buffer, &str](const Core::RegexMatch::MatchedData& match)
                         {
                             buffer.push_back(match.convertBasedOn(str));
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        str.regexIterate(std::string("\\w+"),
                         [&buffer, &str](const Core::RegexMatch::MatchedData& match)
                         {
                             buffer.push_back(match.convertBasedOn(str));
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        std::string expr("\\w+");
        str.regexIterate(expr,
                         [&buffer, &str](const Core::RegexMatch::MatchedData& match)
                         {
                             buffer.push_back(match.convertBasedOn(str));
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        std::string_view expr("\\w+");
        str.regexIterate(expr,
                         [&buffer, &str](const Core::RegexMatch::MatchedData& match)
                         {
                             buffer.push_back(match.convertBasedOn(str));
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        const auto expr = "\\w+"_atom;
        str.regexIterate(expr,
                         [&buffer, &str](const Core::RegexMatch::MatchedData& match)
                         {
                             buffer.push_back(match.convertBasedOn(str));
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        str.regexIterate("\\w+"_atom,
                         [&buffer, &str](const Core::RegexMatch::MatchedData& match)
                         {
                             buffer.push_back(match.convertBasedOn(str));
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }

    {
        const auto str = "Hello world! How are you?"_atom;
        StringAtom buffer;
        str.regexIterate(std::string_view("\\w+"),
                         [&buffer, &str](const Core::RegexMatch::MatchedData& match)
                         {
                             buffer.push_back(match.convertBasedOn(str));
                             return true;
                         });
        EXPECT_EQ("HelloworldHowareyou", buffer);
    }
}

TEST(StringTest, BaseString_char_default__RegexReplace)
{
    auto str = "Hello world!"_atom;
    EXPECT_FALSE(str.regexReplace("\\?", ""));
    EXPECT_EQ("Hello world!", str);
    EXPECT_TRUE(str.regexReplace(" ", "_"));
    EXPECT_EQ("Hello_world!", str);
}

#endif

// =================================================================
// ========================== WCHAR_T ==============================
// =================================================================

TEST(StringTest, BaseString_wchar_t_default__Creation)
{
    using Core::WStringAtom;

    {
        const WStringAtom str1 = L"Hello"_atom;
        const WStringAtom str2 = L"Hello"_atom;
        const WStringAtom str3 = L"World"_atom;

        EXPECT_EQ(str1, str2);
        EXPECT_NE(str1, str3);
    }

    {
        const wchar_t* dynamicStr = new wchar_t[128]{ L"World" };
        const WStringAtom str1 = L"Hello"_atom;
        const WStringAtom str2 = WStringAtom::Intern(dynamicStr);
        EXPECT_NE(str1, str2);

        delete[] dynamicStr;
    }

    {
        const wchar_t* dynamicStr = new wchar_t[128]{ L"World" };
        WStringAtom str1 = L"Hello"_atom;
        WStringAtom str2 = WStringAtom::Intern(dynamicStr);
        EXPECT_NE(str1, str2);

        delete[] dynamicStr;
    }

    {
        WStringAtom str1 = L"Hello"_atom;
        WStringAtom str2 = WStringAtom::Intern(std::wstring(L"World"));
        EXPECT_NE(str1, str2);
    }
}

TEST(StringTest, BaseString_wchar_t_default__DefaultCopyAndMove)
{
    using Core::WStringAtom;

    {
        WStringAtom str1 = L"Hello"_atom;
        WStringAtom str2(str1);

        ASSERT_FALSE(str1.isEmpty());
        ASSERT_FALSE(str2.isEmpty());
        EXPECT_EQ(str1.c_str(), str2.c_str());
        EXPECT_TRUE(str1.isStatic());
        EXPECT_TRUE(str2.isStatic());
        EXPECT_EQ(5, str1.size());
        EXPECT_EQ(5, str2.size());
        EXPECT_EQ(L"Hello", str1);
        EXPECT_EQ(L"Hello", str2);
    }

    {
        WStringAtom str1 = L"Hello"_atom;
        WStringAtom str2(std::move(str1));

        ASSERT_TRUE(str1.isEmpty());
        ASSERT_FALSE(str2.isEmpty());
        EXPECT_NE(str1.c_str(), str2.c_str());
        EXPECT_FALSE(str1.isStatic());
        EXPECT_TRUE(str2.isStatic());
        EXPECT_EQ(0, str1.size());
        EXPECT_EQ(5, str2.size());
        EXPECT_EQ(L"Hello", str2);
    }

    {
        WStringAtom str1 = L"Hello";
        WStringAtom str2(str1);

        ASSERT_FALSE(str1.isEmpty());
        ASSERT_FALSE(str2.isEmpty());
        EXPECT_NE(str1.c_str(), str2.c_str());
        EXPECT_TRUE(str1.isDynamic());
        EXPECT_TRUE(str2.isDynamic());
        EXPECT_EQ(5, str1.size());
        EXPECT_EQ(5, str2.size());
        EXPECT_EQ(L"Hello", str1);
        EXPECT_EQ(L"Hello", str2);
    }

    {
        WStringAtom str1 = L"Hello";
        WStringAtom str2(std::move(str1));

        ASSERT_TRUE(str1.isEmpty());
        ASSERT_FALSE(str2.isEmpty());
        EXPECT_NE(str1.c_str(), str2.c_str());
        EXPECT_EQ(nullptr, str1.c_str());
        EXPECT_FALSE(str1.isDynamic());
        EXPECT_TRUE(str2.isDynamic());
        EXPECT_EQ(0, str1.size());
        EXPECT_EQ(5, str2.size());
        EXPECT_EQ(L"Hello", str2);
    }
}

TEST(StringTest, BaseString_wchar_t_default__Comparision)
{
    using Core::WStringAtom;

    {
        const WStringAtom str1 = L"AAA"_atom;
        const WStringAtom str2 = WStringAtom::Intern(std::wstring(L"AAB"));
        EXPECT_NE(str1, str2);

        EXPECT_TRUE(str1 < str2);
        EXPECT_TRUE(str2 > str1);
    }

    {
        const WStringAtom str1 = L"AAA"_atom;

        EXPECT_TRUE(str1 == L"AAA");
        EXPECT_TRUE(L"AAA" == str1);
        EXPECT_TRUE(str1 != L"AAB");
        EXPECT_TRUE(L"AAB" != str1);

        EXPECT_TRUE(str1 < L"AAB");
        EXPECT_TRUE(L"AAB" > str1);

        EXPECT_TRUE(L"AAA" >= str1);
        EXPECT_TRUE(L"AAA" <= str1);
        EXPECT_TRUE(str1 <= L"AAA");
        EXPECT_TRUE(str1 >= L"AAA");
    }

    {
        const WStringAtom str1 = L"AAA"_atom;

        EXPECT_TRUE(str1 == std::wstring(L"AAA"));
        EXPECT_TRUE(std::wstring(L"AAA") == str1);
        EXPECT_TRUE(str1 != std::wstring(L"AAB"));
        EXPECT_TRUE(std::wstring(L"AAB") != str1);

        EXPECT_TRUE(str1 < std::wstring(L"AAB"));
        EXPECT_TRUE(std::wstring(L"AAB") > str1);

        EXPECT_TRUE(std::wstring(L"AAA") >= str1);
        EXPECT_TRUE(std::wstring(L"AAA") <= str1);
        EXPECT_TRUE(str1 <= std::wstring(L"AAA"));
        EXPECT_TRUE(str1 >= std::wstring(L"AAA"));
    }

    const WStringAtom str2 = WStringAtom::Intern(std::wstring(L"AAB"));
    {
        const WStringAtom str1 = L"AAA"_atom;
        EXPECT_NE(str1, str2);

        EXPECT_TRUE(str1 < str2);
        EXPECT_TRUE(str2 > str1);
    }

    {
        EXPECT_EQ(L"Hello"_atom, L"Hello"_atom);
        EXPECT_TRUE(L"Hello"_atom == L"Hello"_atom);
        EXPECT_TRUE(L"Hello"_atom == std::wstring(L"Hello"));
        EXPECT_TRUE(L"Hello"_atom == std::wstring_view(L"Hello").data());
    }

    {
        EXPECT_TRUE(L"Hello"_atom < L"Hello1");
        EXPECT_FALSE(L"Hello"_atom > L"Hello1");
        EXPECT_TRUE(L"Hello"_atom >= L"Hello");
        EXPECT_TRUE(L"Hello"_atom <= L"Hello");
        EXPECT_TRUE(L"Hello"_atom <= L"Hello1");
        EXPECT_TRUE(L"Hello1"_atom >= L"Hello");

        EXPECT_TRUE(L"Hello"_atom < L"Hello1"_atom);
        EXPECT_FALSE(L"Hello"_atom > L"Hello1"_atom);
        EXPECT_TRUE(L"Hello"_atom != L"Hello1"_atom);
        EXPECT_TRUE(L"Hello"_atom <= L"Hello1"_atom);
        EXPECT_TRUE(L"Hello1"_atom >= L"Hello"_atom);
        EXPECT_TRUE(L"Hello"_atom >= L"Hello"_atom);
        EXPECT_TRUE(L"Hello"_atom <= L"Hello"_atom);

        EXPECT_TRUE(L"Hello"_atom != std::wstring(L"Hello1"));
        EXPECT_TRUE(L"Hello"_atom >= std::wstring(L"Hello"));
        EXPECT_TRUE(L"Hello"_atom <= std::wstring(L"Hello"));
        EXPECT_TRUE(L"Hello1"_atom >= std::wstring(L"Hello"));
        EXPECT_TRUE(L"Hello"_atom <= std::wstring(L"Hello1"));

        EXPECT_TRUE(L"Hello"_atom != std::wstring_view(L"Hello1").data());
        EXPECT_TRUE(L"Hello"_atom >= std::wstring_view(L"Hello").data());
        EXPECT_TRUE(L"Hello"_atom <= std::wstring_view(L"Hello").data());
        EXPECT_TRUE(L"Hello1"_atom >= std::wstring(L"Hello").data());
        EXPECT_TRUE(L"Hello"_atom <= std::wstring_view(L"Hello1").data());
    }
}

TEST(StringTest, BaseString_wchar_t_default__InStdSet)
{
    using Core::WStringAtom;

    {
        std::set<WStringAtom> set;
        set.emplace(WStringAtom::Intern(L"Hello"));
        set.emplace(WStringAtom::Intern(L"World"));
        set.emplace(WStringAtom::Intern(L"Hello"));

        EXPECT_TRUE(set.size() == 2);

        auto itHello = set.find(L"Hello"_atom);
        ASSERT_TRUE(itHello != set.end());
        EXPECT_EQ(*itHello, L"Hello");
        EXPECT_EQ(*itHello, L"Hello"_atom);

        set.emplace(L"AAA"_atom);
        EXPECT_TRUE(set.size() == 3);
        auto itAaa = set.find(L"AAA"_atom);
        ASSERT_TRUE(itAaa != set.end());
        EXPECT_EQ(*itAaa, L"AAA");
        EXPECT_EQ(*itAaa, L"AAA"_atom);
    }
}

TEST(StringTest, BaseString_wchar_t_default__InStdUnorderedSet)
{
    using Core::WStringAtom;

    {
        std::unordered_set<WStringAtom> set;
        set.emplace(WStringAtom::Intern(L"Hello"));
        set.emplace(WStringAtom::Intern(L"World"));
        set.emplace(WStringAtom::Intern(L"Hello"));

        EXPECT_TRUE(set.size() == 2);

        auto itHello = set.find(L"Hello"_atom);
        ASSERT_TRUE(itHello != set.end());
        EXPECT_EQ(*itHello, L"Hello");
        EXPECT_EQ(*itHello, L"Hello"_atom);

        set.emplace(L"AAA"_atom);
        EXPECT_TRUE(set.size() == 3);
        auto itAaa = set.find(L"AAA"_atom);
        ASSERT_TRUE(itAaa != set.end());
        EXPECT_EQ(*itAaa, L"AAA");
        EXPECT_EQ(*itAaa, L"AAA"_atom);
    }
}

TEST(StringTest, BaseString_wchar_t_default__Converts)
{
    using Core::WStringAtom;

    {
        const WStringAtom str = L"123"_atom;
        EXPECT_EQ(123, str.convertTo<int>());
    }

    {
        const WStringAtom str = L"123.123"_atom;
        EXPECT_FLOAT_EQ(123.123f, str.convertTo<float>());
    }

    {
        const WStringAtom str = L"1231234567"_atom;
        EXPECT_EQ(1231234567, str.convertTo<long long>());
    }

    {
        const WStringAtom str = L"f1231234567"_atom;
        EXPECT_EQ(0, str.convertTo<long long>());
    }

    {
        const WStringAtom str = L"1231234567f"_atom;
        EXPECT_EQ(1231234567, str.convertTo<long long>());
    }
}

TEST(StringTest, BaseString_wchar_t_default__UtilsFunctions)
{
    using Core::WStringAtom;

    // Split
    {
        const WStringAtom str = L"Hello fucking world!"_atom;
        auto tokens = str.split(L" L");
        ASSERT_EQ(3, tokens.size());
        EXPECT_EQ(tokens[0], L"Hello");
        EXPECT_EQ(tokens[1], L"fucking");
        EXPECT_EQ(tokens[2], L"world!");
    }
}

TEST(StringTest, BaseString_wchar_t_default__Iterator)
{
    using Core::WStringAtom;

    {
        WStringAtom str = L"Hello world!"_atom;
        auto i = str.begin();
        EXPECT_EQ('H', *i);
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        auto i = str.cbegin();
        EXPECT_EQ('H', *i);
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        auto ci = str.cbegin() + 1;
        auto i = str.begin() + 1;
        EXPECT_EQ('e', *ci);
        EXPECT_EQ('e', *i);
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        auto i = str.end() - 1;
        EXPECT_EQ('!', *i);
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        auto i = str.rbegin() + 1;
        EXPECT_EQ('!', *i);
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        auto front = str.front();
        EXPECT_EQ('H', front);

        auto back = str.back();
        EXPECT_EQ('!', back);
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        auto begin = str.begin();
        auto end = str.end();

        EXPECT_TRUE(begin != end);
        EXPECT_FALSE(begin == end);
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        std::wstring buff;

        for (auto ch : str)
        {
            buff.push_back(ch);
        }
        EXPECT_EQ(str, buff);
    }
}

TEST(StringTest, BaseString_wchar_t_default_Modifications_RangeBasedFor)
{
    using Core::WStringAtom;

    {
        WStringAtom str = L"Hello world!"_atom;
        std::wstring tempStdStr;
        for (auto ch : str)
        {
            tempStdStr.push_back(ch);
        }

        EXPECT_EQ(tempStdStr, str);
    }
}

TEST(StringTest, BaseString_wchar_t_default_Modifications_SubStr)
{
    using Core::WStringAtom;

    {
        WStringAtom str = L"Hello world!"_atom;
        str.subStr(6);
        EXPECT_EQ(L"world!", str);
        EXPECT_EQ(6, str.size());
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        str.subStr(0, 5);
        EXPECT_EQ(L"Hello", str);
        EXPECT_EQ(5, str.size());
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        str.subStr(2, 5);
        EXPECT_EQ(L"llo", str);
        EXPECT_EQ(3, str.size());
    }
}

TEST(StringTest, BaseString_wchar_t_default_Find)
{
    using Core::WStringAtom;

    {
        const auto str = L"Hello world!"_atom;
        const auto* found = str.find(L" ");
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = L"Hello world!"_atom;
        const auto* found = str.find(std::wstring(L" "));
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = L"Hello world!"_atom;
        const auto* found = str.find(L" "_atom);
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = L"Hello world! How are you, world?"_atom;
        const auto strings = str.findAll(L"world");
        ASSERT_FALSE(strings.empty());
        for (const auto& string : strings)
        {
            EXPECT_EQ(L"world", std::wstring_view(string, 5));
        }
    }
}

TEST(StringTest, BaseString_wchar_t_default_Cmp)
{
    using Core::WStringAtom;

    {
        const auto str = L"Hello world!"_atom;
        EXPECT_TRUE(str.compare(L"hello world!", true) == Core::Comparison::Equal);
        EXPECT_TRUE(str.compare(L"hello world", true) == Core::Comparison::Less);
    }

    {
        const auto str = L"AAA"_atom;
        EXPECT_TRUE(str.compare(L"bbb", true) == Core::Comparison::Less);
    }
}

TEST(StringTest, BaseString_wchar_t_default_Trim)
{
    using Core::WStringAtom;

    {
        auto str = L"  MyLogin"_atom;
        str.trimStart(' ');
        EXPECT_EQ(L"MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = L"__MyLogin"_atom;
        str.trimStart(' ');
        EXPECT_EQ(L"__MyLogin", str);
        EXPECT_EQ(9, str.size());
    }

    {
        auto str = L"MyLogin  "_atom;
        str.trimEnd(' ');
        EXPECT_EQ(L"MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = L"MyLogin__"_atom;
        str.trimEnd(' ');
        EXPECT_EQ(L"MyLogin__", str);
        EXPECT_EQ(9, str.size());
    }

    {
        auto str = L"MyLogin  "_atom;
        str.trim(' ');
        EXPECT_EQ(L"MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = L"  MyLogin  "_atom;
        str.trim(' ');
        EXPECT_EQ(L"MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = L"MyLogin__"_atom;
        str.trim(' ');
        EXPECT_EQ(L"MyLogin__", str);
        EXPECT_EQ(9, str.size());
    }

    {
        auto str = L"MyLogin;"_atom;
        str.trim(';');
        EXPECT_EQ(L"MyLogin", str);
        EXPECT_EQ(7, str.size());
    }

    {
        auto str = L"   "_atom;
        str.trim(' ');
        EXPECT_EQ(L"", str);
        EXPECT_EQ(0, str.size());
        EXPECT_NE(0, str.capacity());
    }

    {
        auto str = L"   "_atom;
        str.trimEnd(' ');
        EXPECT_EQ(L"", str);
        EXPECT_EQ(0, str.size());
        EXPECT_NE(0, str.capacity());
    }
}

TEST(StringTest, BaseString_wchar_t_default_ToLower)
{
    {
        auto str = L"Hello World!"_atom;
        str.toLowerCase();
        EXPECT_EQ(L"hello world!", str);
        EXPECT_EQ(12, str.size());
    }
}

TEST(StringTest, BaseString_wchar_t_default_ToUpper)
{
    {
        auto str = L"Hello World!"_atom;
        str.toUpperCase();
        EXPECT_EQ(L"HELLO WORLD!", str);
        EXPECT_EQ(12, str.size());
    }
}

TEST(StringTest, BaseString_wchar_t_default_PushBack)
{
    {
        auto str = L"Hello World"_atom;
        str.push_back('!');
        EXPECT_EQ(L"Hello World!", str);
        EXPECT_EQ(12, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        const auto* text =
            LR"(Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum)";
        str.push_back(text);
        EXPECT_EQ(
            LR"(Hello WorldLorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum)",
            str);
        EXPECT_EQ(11 + wcslen(text), str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.push_back(std::wstring(L"!!!"));
        EXPECT_EQ(L"Hello World!!!", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.push_back(std::wstring(L"!!!"));
        EXPECT_EQ(L"Hello World!!!", str);
        EXPECT_EQ(14, str.size());
    }
}

TEST(StringTest, BaseString_wchar_t_default_PushFront)
{
    {
        auto str = L"Hello World"_atom;
        str.push_front('!');
        EXPECT_EQ(L"!Hello World", str);
        EXPECT_EQ(12, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        const auto* text =
            LR"(Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum)";
        str.push_front(text);
        EXPECT_EQ(
            LR"(Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem IpsumHello World)",
            str);
        EXPECT_EQ(11 + wcslen(text), str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.push_front(std::wstring(L"!!!"));
        EXPECT_EQ(L"!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.push_front(std::wstring(L"!!!"));
        EXPECT_EQ(L"!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }
}

TEST(StringTest, BaseString_wchar_t_default_Insert)
{
    {
        auto str = L"Hello World"_atom;
        str.insert(0, L"!!!");
        EXPECT_EQ(L"!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.insert(0, L"!!!");
        EXPECT_EQ(L"!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.insert(str.size(), L"!!!");
        EXPECT_EQ(L"Hello World!!!", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.insert(str.size(), L"!!!");
        EXPECT_EQ(L"Hello World!!!", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.insert(5, L"!!!");
        EXPECT_EQ(L"Hello!!! World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.insert(5, L"!!!");
        EXPECT_EQ(L"Hello!!! World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.insert(str.begin(), L"!!!");
        EXPECT_EQ(L"!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }

    {
        auto str = L"Hello World"_atom;
        str.insert(str.begin(), L"!!!");
        EXPECT_EQ(L"!!!Hello World", str);
        EXPECT_EQ(14, str.size());
    }
}

TEST(StringTest, BaseString_wchar_t_default_PopBack)
{
    {
        auto str = L"Hello World!"_atom;
        EXPECT_EQ(12, str.size());

        str.pop_back();

        EXPECT_EQ(L"Hello World", str);
        EXPECT_EQ(11, str.size());
    }

    {
        auto str = L"Hello World!"_atom;
        EXPECT_EQ(12, str.size());

        str.pop_back();

        EXPECT_EQ(L"Hello World", str);
        EXPECT_EQ(11, str.size());
    }
}

TEST(StringTest, BaseString_wchar_t_default_PopFront)
{
    {
        auto str = L"Hello World!"_atom;
        EXPECT_EQ(12, str.size());
        std::wstring s;
        str.pop_front();

        EXPECT_EQ(L"ello World!", str);
        EXPECT_EQ(11, str.size());
    }

    {
        auto str = L"Hello World!"_atom;
        EXPECT_EQ(12, str.size());
        std::wstring s;
        str.pop_front();

        EXPECT_EQ(L"ello World!", str);
        EXPECT_EQ(11, str.size());
    }
}

TEST(StringTest, BaseString_wchar_t_default_ShrinkToFit)
{
    {
        auto str = L"Hello World!"_atom;
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(12 + 1, str.capacity());

        str.reserve(100);
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(201, str.capacity());

        str.shrink_to_fit();
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(12 + 1, str.capacity());
    }

    {
        auto str = L"Hello World!"_atom;
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(12 + 1, str.capacity());

        str.reserve(100);
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(201, str.capacity());

        str.shrink_to_fit();
        EXPECT_EQ(12, str.size());
        EXPECT_EQ(12 + 1, str.capacity());
    }
}

TEST(StringTest, BaseString_wchar_t_default_Replace)
{
    {
        auto str = L"Hello World! Hello World!"_atom;
        str.replaceFirst(L"llo", L"LLO___LLO");
        EXPECT_EQ(L"HeLLO___LLO World! Hello World!", str);
    }

    {
        auto str = L"Hello World! Hello World!"_atom;
        str.replaceAll(L"o", L"!o!");
        EXPECT_EQ(L"Hell!o! W!o!rld! Hell!o! W!o!rld!", str);
    }
}

TEST(StringTest, BaseString_wchar_t_default_Copy)
{
    {
        const auto str = L"Hello world!"_atom;
        wchar_t arr[128]{};
        str.copyTo(arr, str.size());
        EXPECT_EQ(str, arr);
    }
}

TEST(StringTest, BaseString_wchar_t_AdvanceWorkFlow)
{
    auto str = L"Hello World!"_atom;

    {
        auto tokens = str.split(L" L");
        ASSERT_EQ(2, tokens.size());
        EXPECT_EQ(L"Hello", tokens[0]);
        EXPECT_EQ(L"World!", tokens[1]);
    }

    str.toLowerCase();
    ASSERT_EQ(L"hello world!", str);
    ASSERT_EQ(str, L"hello world!");
    ASSERT_EQ(std::wstring(L"hello world!"), str);
    ASSERT_EQ(str, std::wstring(L"hello world!"));

    str.toUpperCase();
    ASSERT_EQ(L"HELLO WORLD!", str);
    ASSERT_EQ(str, L"HELLO WORLD!");
    ASSERT_EQ(std::wstring(L"HELLO WORLD!"), str);
    ASSERT_EQ(str, std::wstring(L"HELLO WORLD!"));

    str.replaceAll(L"!", L"???");
    ASSERT_EQ(L"HELLO WORLD???", str);
    ASSERT_EQ(str, L"HELLO WORLD???");
    ASSERT_EQ(std::wstring(L"HELLO WORLD???"), str);
    ASSERT_EQ(str, std::wstring(L"HELLO WORLD???"));
    ASSERT_TRUE(wcscmp(str.c_str(), L"HELLO WORLD???") == 0);

    ASSERT_TRUE(str.compare(L"AAA", true) == Core::Comparison::Greater);
    ASSERT_TRUE(str.compare(L"AAA") == Core::Comparison::Greater);
    ASSERT_TRUE(str.compare(L"aaa") == Core::Comparison::Less);

    str.trim('?');
    ASSERT_EQ(str, L"HELLO WORLD");
}

TEST(StringTest, BaseString_wchar_t_AdvanceWorkFlow2)
{
    Core::WStringAtom str(128);
    ASSERT_TRUE(str.isEmpty());
    ASSERT_TRUE(!str);
    if (!str)
    {
        str.push_back(L"Hello");
        ASSERT_FALSE(str.isEmpty());
        str.push_back(L"World");
        ASSERT_FALSE(str.isEmpty());
        ASSERT_EQ(L"HelloWorld", str);
    }
}

TEST(StringTest, BaseString_wchar_t_AdvanceWorkFlow3)
{
    Core::WStringAtom str;
    str.resize(5);
    if (!str)
    {
        str.push_back(L"Hello");
        ASSERT_FALSE(str.isEmpty());
        str.push_back(L"World");
        ASSERT_FALSE(str.isEmpty());
        ASSERT_EQ(L"     HelloWorld", str);
    }
}

TEST(StringTest, BaseString_wchar_t_to_char)
{
    const auto wstr = L"Hello world!"_atom;
    const auto str = wstr.toASCII();
    EXPECT_EQ("Hello world!", str);
    EXPECT_EQ(12, str.size());
}

TEST(StringTest, BaseString_wchar_t_default__From)
{
    using Core::WStringAtom;
    EXPECT_EQ(L"123", WStringAtom::MakeFrom(123));
    EXPECT_EQ(L"123.000000", WStringAtom::MakeFrom(123.f));
    EXPECT_EQ(L"123.000000", WStringAtom::MakeFrom(123.));
    EXPECT_EQ(L"412312334234", WStringAtom::MakeFrom(412312334234ull));
}

/*TEST(StringTest, BaseString_wchar_t_default__Format)
{
    using Core::WStringAtom;
    const auto str = WStringAtom::Format(L"Hello {}! I have {}$. If u want we can go to {}.", L"Jenny", 300, L"caffee");
    EXPECT_EQ(L"Hello Jenny! I have 300$. If u want we can go to caffee.", str);
}*/

TEST(StringTest, BaseString_wchar_t_default__LinesCount)
{
    using Core::WStringAtom;
    const auto str =
        LR"(Hello
World!
How are you?)"_atom;
    EXPECT_EQ(3, WStringAtom::GetLinesCountInText(str.c_str(), str.c_str() + str.size()));
}

TEST(StringTest, BaseString_wchar_t_default__iterate_over_lines)
{
    using Core::WStringAtom;
    const auto str = L"Hello\nWorld!\nHow are you?"_atom;

    std::vector<WStringAtom> lines = { L"Hello", L"World!", L"How are you?" };

    std::vector<bool> linesValidator;
    int i = 0;
    str.forEachByLine(
        [&](auto str)
        {
            linesValidator.push_back(str == lines[i++]);
        },
        WStringAtom::LineSeparator::LF);

    ASSERT_EQ(lines.size(), linesValidator.size());

    for (auto isValid : linesValidator)
    {
        EXPECT_TRUE(isValid);
    }
}

TEST(StringTest, BaseString_wchar_t_default__Erase)
{
    using Core::WStringAtom;
    {
        auto str = L"Hello world!"_atom;
        str.erase(5);
        EXPECT_EQ(L"Helloworld!", str);
    }

    {
        auto str = L"Hello world!"_atom;
        str.erase(5, 7);
        EXPECT_EQ(L"Hellorld!", str);
    }

    {
        auto str = L"Hello world!"_atom;
        str.erase(str.begin() + 5);
        EXPECT_EQ(L"Helloworld!", str);
    }

    {
        auto str = L"Hello world!"_atom;
        str.erase(str.begin() + 5, str.begin() + 7);
        EXPECT_EQ(L"Hellorld!", str);
    }
}

TEST(StringTest, BaseString_wchar_t_addition)
{
    EXPECT_EQ(L"Hello world!", L"Hello " + Core::WStringAtom(L"world!"));
    EXPECT_EQ(L"Hello world!", L"Hello " + L"world!"_atom);
    EXPECT_EQ(L"Hello world!", L"Hello " + L"world"_atom + L"!");
    EXPECT_EQ(L"Hello world!", L"Hello " + L"world"_atom + L"!"_atom);
    EXPECT_EQ(L"Hello world!", Core::WStringAtom(L"Hello ") + L"world!");
}

TEST(StringTest, BaseString_wchar_t_foreachbyline)
{
    using namespace Core;

    WStringAtom str = L"Hello\nWorld\n!";
    std::vector<WStringAtom> tokens;
    str.forEachByLine(
        [&tokens](auto str)
        {
            tokens.emplace_back(std::move(str));
        });

    ASSERT_EQ(3, tokens.size());
    EXPECT_EQ(L"Hello", tokens[0]);
    EXPECT_EQ(L"World", tokens[1]);
    EXPECT_EQ(L"!", tokens[2]);
}

TEST(StringTest, BaseString_wchar_t_FindNextLine)
{
    using namespace Core;

    WStringAtom str = L"Hello\nWorld\n!";
    std::vector<WStringAtom> tokens = { L"Hello", L"World", L"!" };

    const auto* ptr = str.c_str();

    const auto firstLineStr = WStringAtom(ptr, tokens[0].size());
    ASSERT_TRUE(firstLineStr);
    ASSERT_EQ(L"Hello", firstLineStr);

    const auto secondLine = WStringAtom::FindNextLine(ptr);
    ASSERT_TRUE(secondLine);
    const auto secondLineStr = WStringAtom(secondLine, tokens[1].size());
    ASSERT_EQ(L"World", secondLineStr);
    ptr = secondLine;

    const auto thirdLine = WStringAtom::FindNextLine(ptr);
    ASSERT_TRUE(thirdLine);
    const auto thirdLineStr = WStringAtom(thirdLine, tokens[2].size());
    ASSERT_EQ(L"!", thirdLineStr);
    ptr = thirdLine;

    EXPECT_EQ(nullptr, WStringAtom::FindNextLine(ptr));
}

TEST(StringTest, BaseString_wchar_t_ReverseStrStr)
{
    using namespace Core;

    const auto lenHello = WStringAtom(L"Hello").length();

    {
        WStringAtom str = L"HelloHello";
        EXPECT_EQ(str.c_str() + lenHello, str.reverseFind(L"Hello"));
    }

    {
        WStringAtom str = L"HelloHello";
        EXPECT_EQ(str.c_str(), str.reverseFind(L"Hello", 0, 1));
    }

    {
        WStringAtom str = L"";
        EXPECT_EQ(nullptr, str.reverseFind(L"Hello", 0, 1));
    }
}

TEST(StringTest, BaseString_wchar_t_FindPrevLine)
{
    using namespace Core;

    WStringAtom str = L"Hello\nWorld\n!";
    std::vector<WStringAtom> tokens = { L"Hello", L"World", L"!" };

    const auto* end = str.c_str() + str.size();

    const auto* ptr = WStringAtom::FindPrevLine(str.c_str());
    ASSERT_TRUE(ptr);
    const auto thirdLine = WStringAtom(ptr, tokens[2].size());
    EXPECT_EQ(L"!", thirdLine);

    ptr = WStringAtom::FindPrevLine(str.c_str(), ptr - 1);
    ASSERT_TRUE(ptr);
    const auto secondLine = WStringAtom(ptr, tokens[1].size());
    EXPECT_EQ(L"World", secondLine);

    ptr = WStringAtom::FindPrevLine(str.c_str(), ptr - 1);
    ASSERT_TRUE(ptr);
    const auto firstLine = WStringAtom(ptr, tokens[0].size());
    EXPECT_EQ(L"Hello", firstLine);
}

#ifdef UTILS__USE_STD_REGEX

TEST(StringTest, BaseString_wchar_t_default_Regex)
{
    {
        // check for pascal case
        auto str = L"RegEx"_atom;
        EXPECT_TRUE(str.RegexMatch(L"^([A-Z][a-z0-9]+)+$"));
    }

    {
        auto str = L"RegEx"_atom;
        Core::WStringAtom::StdRegexMatchResults match;
        EXPECT_TRUE(str.RegexMatch(L"^([A-Z][a-z0-9]+)+$", match));
        EXPECT_FALSE(match.empty());
    }

    {
        auto str = L"Hello this fucking world!"_atom;
        str.RegexReplace(L" ", L"_=_");
        EXPECT_EQ(L"Hello_=_this_=_fucking_=_world!", str);
    }
}

TEST(StringTest, BaseString_wchar_t_RegexFind)
{
    using Core::WStringAtom;

    {
        WStringAtom str = L"Hello world!"_atom;
        const auto match = str.RegexFind(L" \\w+");
        EXPECT_EQ(L" world", match.str());
    }

    {
        WStringAtom str = L"Hello world!"_atom;
        const auto match = str.RegexFind(L" \\w+", 3);
        EXPECT_EQ(L" world", match.str());
    }

    {
        WStringAtom str = L"Hello world!";
        const auto match = str.RegexFind(L" \\w+");
        EXPECT_EQ(L" world", match.str());
    }

    {
        WStringAtom str = L"Hello world!";
        const auto match = str.RegexFind(L" \\w+", 3);
        EXPECT_EQ(L" world", match.str());
    }

    {
        WStringAtom str = L"Hello world!";
        const auto match = str.RegexFind(L" \\w+");
        EXPECT_EQ(L" world", match.str());
    }

    {
        WStringAtom str = L"Hello world!";
        const auto match = str.RegexFind(L" \\w+", 3);
        EXPECT_EQ(L" world", match.str());
    }
}

TEST(StringTest, BaseString_wchar_t_RegexIterate)
{
    using Core::WStringAtom;

    {
        const auto str = L"Hello world! How are you?"_atom;
        WStringAtom buffer;
        str.RegexIterate(L"\\w+",
                         [&buffer](const WStringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ(L"HelloworldHowareyou", buffer);
    }

    {
        const auto str = L"Hello world! How are you?"_atom;
        WStringAtom buffer;
        str.RegexIterate(std::wstring(L"\\w+"),
                         [&buffer](const WStringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ(L"HelloworldHowareyou", buffer);
    }

    {
        const auto str = L"Hello world! How are you?"_atom;
        WStringAtom buffer;
        std::wstring expr(L"\\w+");
        str.RegexIterate(expr,
                         [&buffer](const WStringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ(L"HelloworldHowareyou", buffer);
    }

    {
        const auto str = L"Hello world! How are you?"_atom;
        WStringAtom buffer;
        std::wstring_view expr(L"\\w+");
        str.RegexIterate(expr,
                         [&buffer](const WStringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ(L"HelloworldHowareyou", buffer);
    }

    {
        const auto str = L"Hello world! How are you?"_atom;
        WStringAtom buffer;
        const auto expr = L"\\w+"_atom;
        str.RegexIterate(expr,
                         [&buffer](const WStringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ(L"HelloworldHowareyou", buffer);
    }

    {
        const auto str = L"Hello world! How are you?"_atom;
        WStringAtom buffer;
        str.RegexIterate(L"\\w+"_atom,
                         [&buffer](const WStringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ(L"HelloworldHowareyou", buffer);
    }

    {
        const auto str = L"Hello world! How are you?"_atom;
        WStringAtom buffer;
        str.RegexIterate(std::wstring_view(L"\\w+"),
                         [&buffer](const WStringAtom::StdRegexMatchResults& match)
                         {
                             buffer.PushBack(match.str());
                             return true;
                         });
        EXPECT_EQ(L"HelloworldHowareyou", buffer);
    }
}

#endif