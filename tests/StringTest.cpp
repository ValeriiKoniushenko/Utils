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
#include <unordered_set>

TEST(StringTest, BaseString_char_default__Creation)
{
    using Core::StringAtom;

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

        ASSERT_FALSE(str1.IsEmpty());
        ASSERT_FALSE(str2.IsEmpty());
        EXPECT_EQ(str1.Data(), str2.Data());
        EXPECT_TRUE(str1.IsStatic());
        EXPECT_TRUE(str2.IsStatic());
        EXPECT_EQ(5, str1.Size());
        EXPECT_EQ(5, str2.Size());
        EXPECT_EQ("Hello", str1);
        EXPECT_EQ("Hello", str2);
    }

    {
        StringAtom str1 = "Hello"_atom;
        StringAtom str2(std::move(str1));

        ASSERT_TRUE(str1.IsEmpty());
        ASSERT_FALSE(str2.IsEmpty());
        EXPECT_NE(str1.Data(), str2.Data());
        EXPECT_FALSE(str1.IsStatic());
        EXPECT_TRUE(str2.IsStatic());
        EXPECT_EQ(0, str1.Size());
        EXPECT_EQ(5, str2.Size());
        EXPECT_EQ("Hello", str2);
    }

    {
        StringAtom str1 = "Hello";
        StringAtom str2(str1);

        ASSERT_FALSE(str1.IsEmpty());
        ASSERT_FALSE(str2.IsEmpty());
        EXPECT_NE(str1.Data(), str2.Data());
        EXPECT_TRUE(str1.IsDynamic());
        EXPECT_TRUE(str2.IsDynamic());
        EXPECT_EQ(5, str1.Size());
        EXPECT_EQ(5, str2.Size());
        EXPECT_EQ("Hello", str1);
        EXPECT_EQ("Hello", str2);
    }

    {
        StringAtom str1 = "Hello";
        StringAtom str2(std::move(str1));

        ASSERT_TRUE(str1.IsEmpty());
        ASSERT_FALSE(str2.IsEmpty());
        EXPECT_NE(str1.Data(), str2.Data());
        EXPECT_EQ(nullptr, str1.Data());
        EXPECT_FALSE(str1.IsDynamic());
        EXPECT_TRUE(str2.IsDynamic());
        EXPECT_EQ(0, str1.Size());
        EXPECT_EQ(5, str2.Size());
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
        EXPECT_EQ(123, str.ConvertTo<int>());
    }

    {
        const StringAtom str = "123.1234"_atom;
        EXPECT_EQ(123.1234f, str.ConvertTo<float>());
    }

    {
        const StringAtom str = "1231234567"_atom;
        EXPECT_EQ(1231234567, str.ConvertTo<long long>());
    }

    {
        const StringAtom str = "f1231234567"_atom;
        EXPECT_EQ(0, str.ConvertTo<long long>());
    }

    {
        const StringAtom str = "1231234567f"_atom;
        EXPECT_EQ(1231234567, str.ConvertTo<long long>());
    }
}

TEST(StringTest, BaseString_char_default__UtilsFunctions)
{
    using Core::StringAtom;

    // Split
    {
        const StringAtom str = "Hello fucking world!"_atom;
        auto tokens = str.Split(" ");
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
        auto front = str.Front();
        EXPECT_EQ('H', front);

        auto back = str.Back();
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
        str.SubStr(6);
        EXPECT_EQ("world!", str);
        EXPECT_EQ(6, str.Size());
    }

    {
        StringAtom str = "Hello world!"_atom;
        str.SubStr(0, 5);
        EXPECT_EQ("Hello", str);
        EXPECT_EQ(5, str.Size());
    }

    {
        StringAtom str = "Hello world!"_atom;
        str.SubStr(2, 5);
        EXPECT_EQ("llo", str);
        EXPECT_EQ(3, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_Find)
{
    using Core::StringAtom;

    {
        const auto str = "Hello world!"_atom;
        const auto* found = str.Find(" ");
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = "Hello world!"_atom;
        const auto* found = str.Find(std::string(" "));
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = "Hello world!"_atom;
        const auto* found = str.Find(" "_atom);
        ASSERT_TRUE(found);
        EXPECT_EQ(' ', *found);
    }

    {
        const auto str = "Hello world! How are you, world?"_atom;
        const auto strings = str.FindAll("world");
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
        EXPECT_TRUE(str.Compare("hello world!", true) == Core::Comparison::Equal);
        EXPECT_TRUE(str.Compare("hello world", true) == Core::Comparison::Less);
    }

    {
        const auto str = "AAA"_atom;
        EXPECT_TRUE(str.Compare("bbb", true) == Core::Comparison::Less);
    }
}

TEST(StringTest, BaseString_char_default_Trim)
{
    using Core::StringAtom;

    {
        auto str = "  MyLogin"_atom;
        str.TrimStart(' ');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.Size());
    }

    {
        auto str = "__MyLogin"_atom;
        str.TrimStart(' ');
        EXPECT_EQ("__MyLogin", str);
        EXPECT_EQ(9, str.Size());
    }

    {
        auto str = "MyLogin  "_atom;
        str.TrimEnd(' ');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.Size());
    }

    {
        auto str = "MyLogin__"_atom;
        str.TrimEnd(' ');
        EXPECT_EQ("MyLogin__", str);
        EXPECT_EQ(9, str.Size());
    }

    {
        auto str = "MyLogin  "_atom;
        str.Trim(' ');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.Size());
    }

    {
        auto str = "  MyLogin  "_atom;
        str.Trim(' ');
        EXPECT_EQ("MyLogin", str);
        EXPECT_EQ(7, str.Size());
    }

    {
        auto str = "MyLogin__"_atom;
        str.Trim(' ');
        EXPECT_EQ("MyLogin__", str);
        EXPECT_EQ(9, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_ToLower)
{
    {
        auto str = "Hello World!"_atom;
        str.ToLowerCase();
        EXPECT_EQ("hello world!", str);
        EXPECT_EQ(12, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_ToUpper)
{
    {
        auto str = "Hello World!"_atom;
        str.ToUpperCase();
        EXPECT_EQ("HELLO WORLD!", str);
        EXPECT_EQ(12, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_PushBack)
{
    {
        auto str = "Hello World"_atom;
        str.push_back('!');
        EXPECT_EQ("Hello World!", str);
        EXPECT_EQ(12, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        const auto* text =
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
        str.push_back(text);
        EXPECT_EQ(
            "Hello WorldLorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum",
            str);
        EXPECT_EQ(11 + strlen(text), str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.push_back(std::string("!!!"));
        EXPECT_EQ("Hello World!!!", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.PushBack(std::string("!!!"));
        EXPECT_EQ("Hello World!!!", str);
        EXPECT_EQ(14, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_PushFront)
{
    {
        auto str = "Hello World"_atom;
        str.push_front('!');
        EXPECT_EQ("!Hello World", str);
        EXPECT_EQ(12, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        const auto* text =
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
        str.push_front(text);
        EXPECT_EQ(
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem IpsumHello World",
            str);
        EXPECT_EQ(11 + strlen(text), str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.push_front(std::string("!!!"));
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.PushFront(std::string("!!!"));
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_Insert)
{
    {
        auto str = "Hello World"_atom;
        str.insert(0, "!!!");
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.Insert(0, "!!!");
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(str.Size(), "!!!");
        EXPECT_EQ("Hello World!!!", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.Insert(str.Size(), "!!!");
        EXPECT_EQ("Hello World!!!", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(5, "!!!");
        EXPECT_EQ("Hello!!! World", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.Insert(5, "!!!");
        EXPECT_EQ("Hello!!! World", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.insert(str.begin(), "!!!");
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.Size());
    }

    {
        auto str = "Hello World"_atom;
        str.Insert(str.begin(), "!!!");
        EXPECT_EQ("!!!Hello World", str);
        EXPECT_EQ(14, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_PopBack)
{
    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.Size());

        str.pop_back();

        EXPECT_EQ("Hello World", str);
        EXPECT_EQ(11, str.Size());
    }

    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.Size());

        str.PopBack();

        EXPECT_EQ("Hello World", str);
        EXPECT_EQ(11, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_PopFront)
{
    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.Size());
        std::string s;
        str.pop_front();

        EXPECT_EQ("ello World!", str);
        EXPECT_EQ(11, str.Size());
    }

    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.Size());
        std::string s;
        str.PopFront();

        EXPECT_EQ("ello World!", str);
        EXPECT_EQ(11, str.Size());
    }
}

TEST(StringTest, BaseString_char_default_ShrinkToFit)
{
    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.Size());
        EXPECT_EQ(12 + 1, str.Capacity());

        str.Reserve(100);
        EXPECT_EQ(12, str.Size());
        EXPECT_EQ(201, str.Capacity());

        str.shrink_to_fit();
        EXPECT_EQ(12, str.Size());
        EXPECT_EQ(12 + 1, str.Capacity());
    }

    {
        auto str = "Hello World!"_atom;
        EXPECT_EQ(12, str.Size());
        EXPECT_EQ(12 + 1, str.Capacity());

        str.Reserve(100);
        EXPECT_EQ(12, str.Size());
        EXPECT_EQ(201, str.Capacity());

        str.ShrinkToFit();
        EXPECT_EQ(12, str.Size());
        EXPECT_EQ(12 + 1, str.Capacity());
    }
}

TEST(StringTest, BaseString_char_default_Replace)
{
    {
        auto str = "Hello World! Hello World!"_atom;
        str.ReplaceFirst("llo", "LLO___LLO");
        EXPECT_EQ("HeLLO___LLO World! Hello World!", str);
    }

    {
        auto str = "Hello World! Hello World!"_atom;
        str.ReplaceAll("o", "!o!");
        EXPECT_EQ("Hell!o! W!o!rld! Hell!o! W!o!rld!", str);
    }
}

TEST(StringTest, BaseString_char_default_Regex)
{
    {
        // check for pascal case
        auto str = "RegEx"_atom;
        EXPECT_TRUE(str.RegexMatch("^([A-Z][a-z0-9]+)+$"));
    }
}

TEST(StringTest, BaseString_char_default_Copy)
{
    {
        const auto str = "Hello world!"_atom;
        char arr[128]{};
        str.Copy(arr, str.Size());
        EXPECT_EQ(str, arr);
    }
}

TEST(StringTest, BaseString_char_AdvanceWorkFlow)
{
    auto str = "Hello World!"_atom;

    {
        auto tokens = str.Split(" ");
        ASSERT_EQ(2, tokens.size());
        EXPECT_EQ("Hello", tokens[0]);
        EXPECT_EQ("World!", tokens[1]);
    }

    str.ToLowerCase();
    ASSERT_EQ("hello world!", str);
    ASSERT_EQ(str, "hello world!");
    ASSERT_EQ(std::string("hello world!"), str);
    ASSERT_EQ(str, std::string("hello world!"));

    str.ToUpperCase();
    ASSERT_EQ("HELLO WORLD!", str);
    ASSERT_EQ(str, "HELLO WORLD!");
    ASSERT_EQ(std::string("HELLO WORLD!"), str);
    ASSERT_EQ(str, std::string("HELLO WORLD!"));

    str.ReplaceAll("!", "???");
    ASSERT_EQ("HELLO WORLD???", str);
    ASSERT_EQ(str, "HELLO WORLD???");
    ASSERT_EQ(std::string("HELLO WORLD???"), str);
    ASSERT_EQ(str, std::string("HELLO WORLD???"));
    ASSERT_TRUE(strcmp(str.c_str(), "HELLO WORLD???") == 0);

    ASSERT_TRUE(str.Compare("AAA", true) == Core::Comparison::Greater);
    ASSERT_TRUE(str.Compare("AAA") == Core::Comparison::Greater);
    ASSERT_TRUE(str.Compare("aaa") == Core::Comparison::Less);

    str.Trim('?');
    ASSERT_EQ(str, "HELLO WORLD");
}

TEST(StringTest, BaseString_char_AdvanceWorkFlow2)
{
    Core::StringAtom str(128);
    ASSERT_TRUE(str.IsEmpty());
    ASSERT_TRUE(!str);
    if (!str)
    {
        str.push_back("Hello");
        ASSERT_FALSE(str.IsEmpty());
        str.push_back("World");
        ASSERT_FALSE(str.IsEmpty());
        ASSERT_EQ("HelloWorld", str);
    }
}

TEST(StringTest, BaseString_char_AdvanceWorkFlow3)
{
    Core::StringAtom str;
    str.Resize(5);
    if (!str)
    {
        str.push_back("Hello");
        ASSERT_FALSE(str.IsEmpty());
        str.push_back("World");
        ASSERT_FALSE(str.IsEmpty());
        ASSERT_EQ("     HelloWorld", str);
    }
}
