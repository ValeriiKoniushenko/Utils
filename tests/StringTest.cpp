/*
 * MIT License
 *
 * Copyright (c) 2018-2025 Valerii Koniushenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Core/String.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <unordered_set>

using namespace Core;

class StringTestF : public ::testing::Test
{
public:
    template<class T>
    const T* Str(const char* s)
    {
        if constexpr (std::is_same_v<T, char>)
        {
            return s;
        }
        else
        {
            static std::wstring ws;
            ws = std::wstring(s, s + std::strlen(s));
            return ws.c_str();
        }
    }
};

TEST_F(StringTestF, ConverterToString)
{
    auto test = [this]<class T>()
    {
        using String = BaseString<T>;

        EXPECT_EQ(String("1"), String::MakeFrom('1'));
        EXPECT_EQ(String("123"), String::MakeFrom(123));
        EXPECT_EQ(String("123"), String::MakeFrom(123ll));
        EXPECT_EQ(String("123"), String::MakeFrom(123u));
        EXPECT_EQ(String("123"), String::MakeFrom(123ull));
        EXPECT_EQ(String("123"), String::MakeFrom(123.));
        EXPECT_EQ(String("123"), String::MakeFrom(123.f));
        EXPECT_EQ(String("123.123"), String::MakeFrom(123.123));
        EXPECT_EQ(String("123.123"), String::MakeFrom(123.123f));
        EXPECT_EQ(String("123456789111"), String::MakeFrom(123'456'789'111));
        EXPECT_EQ(String("true"), String::MakeFrom(true));
        EXPECT_EQ(String("false"), String::MakeFrom(false));

        EXPECT_EQ(String("Hello world"), String::MakeFrom(std::basic_string<T>(Str<T>("Hello world"))));
        EXPECT_EQ(String("Hello world"), String::MakeFrom(std::basic_string<T>(Str<T>("Hello world")).c_str()));
        EXPECT_EQ(String("Hello world"), String::MakeFrom(std::basic_string<T>(Str<T>("Hello world")).data()));

        EXPECT_EQ(String("Hello world"), String::MakeFrom(std::basic_string_view<T>(Str<T>("Hello world"))));
        EXPECT_EQ(String("Hello world"), String::MakeFrom(std::basic_string_view<T>(Str<T>("Hello world")).data()));

        EXPECT_EQ(String("path/to/dir"), String::MakeFrom(std::filesystem::path(Str<T>("path/to/dir"))));
        EXPECT_EQ(String("path/to/dir"), String::MakeFrom(std::filesystem::path(Str<T>("path/to/dir")).generic_string<T>()));
        EXPECT_EQ(String("path/to/dir"), String::MakeFrom(std::filesystem::path(Str<T>("path/to/dir")).generic_string<T>().c_str()));
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, ConverterFromString)
{
    auto test = [this]<class T>()
    {
        using TS = StringToolset<T>;
        // floating point
        EXPECT_EQ(123.f, TS::template FromCStringTo<float>(Str<T>("123.f")));
        EXPECT_EQ(123., TS::template FromCStringTo<double>(Str<T>("123.")));

        EXPECT_THROW((void)TS::template FromCStringTo<double>(Str<T>("ss123.")), std::invalid_argument);

        // integral
        // int32 or lower
        EXPECT_EQ(2'123'456'789, TS::template FromCStringTo<int>(Str<T>("2123456789")));
        EXPECT_EQ(123, TS::template FromCStringTo<short>(Str<T>("123")));
        EXPECT_EQ(123, TS::template FromCStringTo<char>(Str<T>("123")));
        EXPECT_EQ(-2'123'456'789, TS::template FromCStringTo<int>(Str<T>("-2123456789")));
        EXPECT_EQ(-123, TS::template FromCStringTo<short>(Str<T>("-123")));
        EXPECT_EQ(-123, TS::template FromCStringTo<char>(Str<T>("-123")));
        EXPECT_EQ(3'123'456'789, TS::template FromCStringTo<unsigned int>(Str<T>("3123456789")));
        EXPECT_EQ(123, TS::template FromCStringTo<unsigned short>(Str<T>("123")));
        EXPECT_EQ(123, TS::template FromCStringTo<unsigned char>(Str<T>("123")));
        // int64
        EXPECT_EQ(123, TS::template FromCStringTo<long long>(Str<T>("123")));
        EXPECT_EQ(112'123'456'789, TS::template FromCStringTo<long long>(Str<T>("112123456789")));
        EXPECT_EQ(-112'123'456'789, TS::template FromCStringTo<long long>(Str<T>("-112123456789")));
        EXPECT_EQ(112'123'456'789, TS::template FromCStringTo<unsigned long long>(Str<T>("112123456789")));
        EXPECT_EQ(112'123'456'789, TS::template FromCStringTo<int64_t>(Str<T>("112123456789")));
        EXPECT_EQ(112'123'456'789, TS::template FromCStringTo<uint64_t>(Str<T>("112123456789")));
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Assigning)
{
    auto test = [this]<class T>()
    {
        {
            BaseString<T> string;
            string = Str<T>("Hello");
            EXPECT_EQ(string, Str<T>("Hello"));
        }

        {
            BaseString<T> string;

            std::basic_string<T> s = Str<T>("Hello");
            string = s;
            EXPECT_EQ(string, Str<T>("Hello"));
        }

        {
            BaseString<T> string;
            string = std::basic_string<T>(Str<T>("Hello"));
            EXPECT_EQ(string, Str<T>("Hello"));
        }

        {
            BaseString<T> string;
            std::basic_string_view<T> s = Str<T>("Hello");
            string = s;
            EXPECT_EQ(string, Str<T>("Hello"));
        }

        {
            BaseString<T> string;
            string = std::basic_string_view<T>(Str<T>("Hello"));
            EXPECT_EQ(string, Str<T>("Hello"));
        }

        {
            BaseString<T> string;
            BaseString<T> s = Str<T>("Hello");
            string = s;
            EXPECT_EQ(string, Str<T>("Hello"));
        }

        {
            BaseString<T> string;
            string = BaseString<T>(Str<T>("Hello"));
            EXPECT_EQ(string, Str<T>("Hello"));
        }

        std::basic_string<T> testStr = Str<T>("Hello");
        EXPECT_EQ(testStr.data(), BaseString<T>(Str<T>("Hello")));
        EXPECT_EQ(testStr.data(), BaseString<T>(std::basic_string<T>(Str<T>("Hello"))));
        EXPECT_EQ(testStr.data(), BaseString<T>(std::basic_string_view<T>(Str<T>("Hello"))));
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Creation)
{
    auto test = [this]<class T>()
    {
        {
            const BaseString<T> str1 = Str<T>("");

            EXPECT_EQ(Str<T>(""), str1);
            EXPECT_EQ(0, str1.size());
            EXPECT_NE(0, str1.capacity());
        }

        {
            const BaseString<T> str1 = BaseString<T>::Intern(Str<T>("Hello"));
            const BaseString<T> str2 = BaseString<T>::Intern(Str<T>("Hello"));
            const BaseString<T> str3 = BaseString<T>::Intern(Str<T>("World"));

            EXPECT_EQ(str1, str2);
            EXPECT_NE(str1, str3);
        }

        {
            T* dynamicStr = new T[128]{};
            ::memcpy_s(dynamicStr, 128 * sizeof(T), Str<T>("World"), strlen("World") * sizeof(T));
            const BaseString<T> str1 = BaseString<T>::Intern(Str<T>("Hello"));
            const BaseString<T> str2 = BaseString<T>::Intern(dynamicStr);
            EXPECT_NE(str1, str2);

            delete[] dynamicStr;
        }

        {
            T* dynamicStr = new T[128]{};
            ::memcpy_s(dynamicStr, 128 * sizeof(T), Str<T>("World"), strlen("World") * sizeof(T));

            BaseString<T> str1 = BaseString<T>::Intern(Str<T>("Hello"));
            BaseString<T> str2 = BaseString<T>::Intern(dynamicStr);
            EXPECT_NE(str1, str2);

            delete[] dynamicStr;
        }

        {
            BaseString<T> str1 = BaseString<T>::Intern(Str<T>("Hello"));
            BaseString<T> str2 = BaseString<T>::Intern(std::basic_string<T>(Str<T>("World")));
            EXPECT_NE(str1, str2);
        }

        {
            BaseString<T> str;
            EXPECT_EQ(str, Str<T>(""));
            EXPECT_NE(str, Str<T>(" "));
            EXPECT_NE(str, Str<T>("smth"));
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, DefaultCopyAndMove)
{
    auto test = [this]<class T>()
    {
        {
            BaseString<T> str1 = BaseString<T>::Intern(Str<T>("Hello"));
            BaseString<T> str2(str1);

            ASSERT_FALSE(str1.isEmpty());
            ASSERT_FALSE(str2.isEmpty());
            EXPECT_EQ(str1.c_str(), str2.c_str());
            EXPECT_TRUE(str1.isStatic());
            EXPECT_TRUE(str2.isStatic());
            EXPECT_EQ(5, str1.size());
            EXPECT_EQ(5, str2.size());
            EXPECT_EQ(Str<T>("Hello"), str1);
            EXPECT_EQ(Str<T>("Hello"), str2);
        }

        {
            BaseString<T> str1 = BaseString<T>::Intern(Str<T>("Hello"));
            BaseString<T> str2(std::move(str1));

            ASSERT_TRUE(str1.isEmpty());
            ASSERT_FALSE(str2.isEmpty());
            EXPECT_NE(str1.c_str(), str2.c_str());
            EXPECT_FALSE(str1.isStatic());
            EXPECT_TRUE(str2.isStatic());
            EXPECT_EQ(0, str1.size());
            EXPECT_EQ(5, str2.size());
            EXPECT_EQ(Str<T>("Hello"), str2);
        }

        {
            BaseString<T> str1 = Str<T>("Hello");
            BaseString<T> str2(str1);

            ASSERT_FALSE(str1.isEmpty());
            ASSERT_FALSE(str2.isEmpty());
            EXPECT_NE(str1.c_str(), str2.c_str());
            EXPECT_TRUE(str1.isDynamic());
            EXPECT_TRUE(str2.isDynamic());
            EXPECT_EQ(5, str1.size());
            EXPECT_EQ(5, str2.size());
            EXPECT_EQ(Str<T>("Hello"), str1);
            EXPECT_EQ(Str<T>("Hello"), str2);
        }

        {
            BaseString<T> str1 = Str<T>("Hello");
            BaseString<T> str2(std::move(str1));

            ASSERT_TRUE(str1.isEmpty());
            ASSERT_FALSE(str2.isEmpty());
            EXPECT_NE(str1.c_str(), str2.c_str());
            EXPECT_EQ(nullptr, str1.c_str());
            EXPECT_FALSE(str1.isDynamic());
            EXPECT_TRUE(str2.isDynamic());
            EXPECT_EQ(0, str1.size());
            EXPECT_EQ(5, str2.size());
            EXPECT_EQ(Str<T>("Hello"), str2);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, WorkingWithEmpty)
{
    auto test = [this]<class T>()
    {
        {
            BaseString<T> str;
            EXPECT_EQ(Str<T>(""), str);
        }

        {
            BaseString<T> str = Str<T>("");
            EXPECT_EQ(Str<T>(""), str);
        }

        {
            BaseString<T> str;
            BaseString<T> str1 = str;

            EXPECT_EQ(Str<T>(""), str);
            EXPECT_EQ(Str<T>(""), str1);
        }

        {
            BaseString<T> str;
            BaseString<T> str1 = std::move(str);

            EXPECT_EQ(Str<T>(""), str1);
        }

        {
            BaseString<T> str;

            std::basic_string<T> str1 = (str + Str<T>("Hello")).data();
            EXPECT_EQ(Str<T>("Hello"), str1);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Comparision)
{
    auto test = [this]<class T>()
    {
        {
            const BaseString<T> str1 = BaseString<T>::Intern(Str<T>("AAA"));
            const BaseString<T> str2 = BaseString<T>::Intern(std::basic_string<T>(Str<T>("AAB")));
            EXPECT_NE(str1, str2);

            EXPECT_TRUE(str1 < str2);
            EXPECT_TRUE(str2 > str1);
        }

        {
            const BaseString<T> str1 = BaseString<T>::Intern(Str<T>("AAA"));

            EXPECT_TRUE(str1 == Str<T>("AAA"));
            EXPECT_TRUE(Str<T>("AAA") == str1);
            EXPECT_TRUE(str1 != Str<T>("AAB"));
            EXPECT_TRUE(Str<T>("AAB") != str1);

            EXPECT_TRUE(str1 < Str<T>("AAB"));
            EXPECT_TRUE(Str<T>("AAB") > str1);

            EXPECT_TRUE(Str<T>("AAA") >= str1);
            EXPECT_TRUE(Str<T>("AAA") <= str1);
            EXPECT_TRUE(str1 <= Str<T>("AAA"));
            EXPECT_TRUE(str1 >= Str<T>("AAA"));
        }

        {
            const BaseString<T> str1 = BaseString<T>::Intern(Str<T>("AAA"));

            EXPECT_TRUE(str1 == std::basic_string<T>(Str<T>("AAA")));
            EXPECT_TRUE(std::basic_string<T>(Str<T>("AAA")) == str1);
            EXPECT_TRUE(str1 != std::basic_string<T>(Str<T>("AAB")));
            EXPECT_TRUE(std::basic_string<T>(Str<T>("AAB")) != str1);

            EXPECT_TRUE(str1 < std::basic_string<T>(Str<T>("AAB")));
            EXPECT_TRUE(std::basic_string<T>(Str<T>("AAB")) > str1);

            EXPECT_TRUE(std::basic_string<T>(Str<T>("AAA")) >= str1);
            EXPECT_TRUE(std::basic_string<T>(Str<T>("AAA")) <= str1);
            EXPECT_TRUE(str1 <= std::basic_string<T>(Str<T>("AAA")));
            EXPECT_TRUE(str1 >= std::basic_string<T>(Str<T>("AAA")));
        }

        const BaseString<T> str2 = BaseString<T>::Intern(std::basic_string<T>(Str<T>("AAB")));
        {
            const BaseString<T> str1 = BaseString<T>::Intern(Str<T>("AAA"));
            EXPECT_NE(str1, str2);

            EXPECT_TRUE(str1 < str2);
            EXPECT_TRUE(str2 > str1);
        }

        std::basic_string<T> hello = Str<T>("Hello");
        std::basic_string<T> hello1 = Str<T>("Hello1");

        {
            EXPECT_EQ(BaseString<T>::Intern(hello.data()), BaseString<T>::Intern(hello.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) == BaseString<T>::Intern(hello.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) == std::basic_string<T>(hello.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) == std::basic_string_view<T>(hello.data()).data());
        }

        {
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) < hello1.data());
            EXPECT_FALSE(BaseString<T>::Intern(hello.data()) > hello1.data());
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) >= hello.data());
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) <= hello.data());
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) <= hello1.data());
            EXPECT_TRUE(BaseString<T>::Intern(hello1.data()) >= hello.data());

            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) < BaseString<T>::Intern(hello1.data()));
            EXPECT_FALSE(BaseString<T>::Intern(hello.data()) > BaseString<T>::Intern(hello1.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) != BaseString<T>::Intern(hello1.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) <= BaseString<T>::Intern(hello1.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello1.data()) >= BaseString<T>::Intern(hello.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) >= BaseString<T>::Intern(hello.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) <= BaseString<T>::Intern(hello.data()));

            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) != std::basic_string<T>(hello1.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) >= std::basic_string<T>(hello.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) <= std::basic_string<T>(hello.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello1.data()) >= std::basic_string<T>(hello.data()));
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) <= std::basic_string<T>(hello1.data()));

            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) != std::basic_string_view<T>(hello1.data()).data());
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) >= std::basic_string_view<T>(hello.data()).data());
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) <= std::basic_string_view<T>(hello.data()).data());
            EXPECT_TRUE(BaseString<T>::Intern(hello1.data()) >= std::basic_string<T>(hello.data()).data());
            EXPECT_TRUE(BaseString<T>::Intern(hello.data()) <= std::basic_string_view<T>(hello1.data()).data());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, OperationsWithEmptyString)
{
    auto test = [this]<class T>()
    {
        {
            BaseString<T> str;
            auto dynamic = str.getCopyAsDynamic();
            EXPECT_EQ(0, dynamic.size());
            EXPECT_NE(0, dynamic.capacity());
            EXPECT_NE(nullptr, dynamic.c_str());
        }

        {
            BaseString<T> str;

            EXPECT_EQ(0, str.makeHash());
            EXPECT_EQ(str.end(), str.begin());
            EXPECT_TRUE(Str<T>("") == str);
            EXPECT_TRUE(str == Str<T>(""));
        }

        {
            BaseString<T> str = Str<T>("");

            EXPECT_EQ(0, str.makeHash());
            EXPECT_EQ(str.end(), str.begin());
            EXPECT_TRUE(Str<T>("") == str);
            EXPECT_TRUE(str == Str<T>(""));
        }

        {
            BaseString<T> str;
            str.push_back(Str<T>("Hello"));
            EXPECT_EQ(5, str.size());
            EXPECT_GE(str.capacity(), 5);
        }

        {
            BaseString<T> str;
            str.push_front(Str<T>("Hello"));
            EXPECT_EQ(5, str.size());
            EXPECT_GE(str.capacity(), 5);
        }

        if constexpr (std::is_same_v<T, char>)
        {
            {
                BaseString<T> str;
                EXPECT_FALSE(str.regexFind(Str<T>("Hello")).isMatched());
            }

            {
                BaseString<T> str;
                EXPECT_TRUE(str.regexFindAll(Str<T>("Hello")).empty());
            }

            {
                BaseString<T> str;
                EXPECT_FALSE(str.regexMatch(Str<T>("Hello")));

                str.shrink_to_fit();
            }
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, InStdSet)
{
    auto test = [this]<class T>()
    {
        {
            std::set<BaseString<T>> set;
            set.emplace(BaseString<T>::Intern(Str<T>("Hello")));
            set.emplace(BaseString<T>::Intern(Str<T>("World")));
            set.emplace(BaseString<T>::Intern(Str<T>("Hello")));

            EXPECT_TRUE(set.size() == 2);

            auto itHello = set.find(BaseString<T>::Intern(Str<T>("Hello")));
            ASSERT_TRUE(itHello != set.end());
            EXPECT_EQ(*itHello, Str<T>("Hello"));
            EXPECT_EQ(*itHello, BaseString<T>::Intern(Str<T>("Hello")));

            set.emplace(BaseString<T>::Intern(Str<T>("AAA")));
            EXPECT_TRUE(set.size() == 3);
            auto itAaa = set.find(BaseString<T>::Intern(Str<T>("AAA")));
            ASSERT_TRUE(itAaa != set.end());
            EXPECT_EQ(*itAaa, Str<T>("AAA"));
            EXPECT_EQ(*itAaa, BaseString<T>::Intern(Str<T>("AAA")));
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, InStdUnorderedSet)
{
    auto test = [this]<class T>()
    {
        {
            std::unordered_set<BaseString<T>> set;
            set.emplace(BaseString<T>::Intern(Str<T>("Hello")));
            set.emplace(BaseString<T>::Intern(Str<T>("World")));
            set.emplace(BaseString<T>::Intern(Str<T>("Hello")));

            EXPECT_TRUE(set.size() == 2);

            auto itHello = set.find(BaseString<T>::Intern(Str<T>("Hello")));
            ASSERT_TRUE(itHello != set.end());
            EXPECT_EQ(*itHello, Str<T>("Hello"));
            EXPECT_EQ(*itHello, BaseString<T>::Intern(Str<T>("Hello")));

            set.emplace(BaseString<T>::Intern(Str<T>("AAA")));
            EXPECT_TRUE(set.size() == 3);
            auto itAaa = set.find(BaseString<T>::Intern(Str<T>("AAA")));
            ASSERT_TRUE(itAaa != set.end());
            EXPECT_EQ(*itAaa, Str<T>("AAA"));
            EXPECT_EQ(*itAaa, BaseString<T>::Intern(Str<T>("AAA")));
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Converts)
{
    auto test = [this]<class T>()
    {
        {
            const BaseString<T> str = BaseString<T>::Intern(Str<T>("123"));
            EXPECT_EQ(123, str.template convertTo<int>());
        }

        {
            const BaseString<T> str = BaseString<T>::Intern(Str<T>("123.1234"));
            EXPECT_FLOAT_EQ(123.1234f, str.template convertTo<float>());
        }

        {
            const BaseString<T> str = BaseString<T>::Intern(Str<T>("1231234567"));
            EXPECT_EQ(1231234567, str.template convertTo<long long>());
        }

        {
            const BaseString<T> str = BaseString<T>::Intern(Str<T>("f1231234567"));
            EXPECT_THROW((void)str.template convertTo<long long>(), std::invalid_argument);
        }

        {
            const BaseString<T> str = BaseString<T>::Intern(Str<T>("1231234567f"));
            EXPECT_EQ(1231234567, str.template convertTo<long long>());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, UtilsFunctions)
{
    auto test = [this]<class T>()
    {
        // Split
        {
            const BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello fucking world!"));
            auto tokens = str.split(Str<T>(" "));
            ASSERT_EQ(3, tokens.size());
            EXPECT_EQ(tokens[0], Str<T>("Hello"));
            EXPECT_EQ(tokens[1], Str<T>("fucking"));
            EXPECT_EQ(tokens[2], Str<T>("world!"));
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, SequentalPushBack)
{
    auto test = [this]<class T>()
    {
        BaseString<T> str;
        ASSERT_EQ(BaseString<T>::minAllocationSize, str.capacity());
        ASSERT_EQ(0, str.size());

        str.push_back(Str<T>("Hello"));
        ASSERT_EQ(5, str.size());
        ASSERT_EQ(BaseString<T>::minAllocationSize, str.capacity());
        EXPECT_EQ(Str<T>("Hello"), str);

        str.push_back(Str<T>("HelloWorldHowAreYou!")); // 20chars
        ASSERT_EQ(25, str.size());
        ASSERT_EQ(BaseString<T>::minAllocationSize, str.capacity());
        EXPECT_EQ(Str<T>("HelloHelloWorldHowAreYou!"), str);

        str.push_back(Str<T>("HelloWorldHowAreYou!")); // 20chars
        ASSERT_EQ(45, str.size());
        ASSERT_EQ(45 * 2, str.capacity());
        EXPECT_EQ(Str<T>("HelloHelloWorldHowAreYou!HelloWorldHowAreYou!"), str);
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, SequentalPushFront)
{
    auto test = [this]<class T>()
    {
        BaseString<T> str;
        ASSERT_EQ(BaseString<T>::minAllocationSize, str.capacity());
        ASSERT_EQ(0, str.size());

        str.push_front(Str<T>("Hello"));
        ASSERT_EQ(5, str.size());
        ASSERT_EQ(BaseString<T>::minAllocationSize, str.capacity());
        EXPECT_EQ(Str<T>("Hello"), str);

        str.push_front(Str<T>("HelloWorldHowAreYou!")); // 20chars
        ASSERT_EQ(25, str.size());
        ASSERT_EQ(BaseString<T>::minAllocationSize, str.capacity());
        EXPECT_EQ(Str<T>("HelloWorldHowAreYou!Hello"), str);

        str.push_front(Str<T>("HelloWorldHowAreYou!")); // 20chars
        ASSERT_EQ(45, str.size());
        ASSERT_EQ(45 * 2, str.capacity());
        EXPECT_EQ(Str<T>("HelloWorldHowAreYou!HelloWorldHowAreYou!Hello"), str);
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Iterator)
{
    auto test = [this]<class T>()
    {
        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            auto i = str.begin();
            EXPECT_EQ('H', *i);
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            auto i = str.cbegin();
            EXPECT_EQ('H', *i);
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            auto ci = str.cbegin() + 1;
            auto i = str.begin() + 1;
            EXPECT_EQ('e', *ci);
            EXPECT_EQ('e', *i);
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            auto i = str.end() - 1;
            EXPECT_EQ('!', *i);
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            auto i = str.rbegin() + 1;
            EXPECT_EQ('!', *i);
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            auto front = str.front();
            EXPECT_EQ('H', front);

            auto back = str.back();
            EXPECT_EQ('!', back);
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            auto begin = str.begin();
            auto end = str.end();

            EXPECT_TRUE(begin != end);
            EXPECT_FALSE(begin == end);
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            std::basic_string<T> buff;

            for (auto ch : str)
            {
                buff.push_back(ch);
            }
            EXPECT_EQ(str, buff);
        }

        if constexpr (std::is_same_v<T, char>)
        {
            std::ofstream out(Str<T>("temp.txt"));
            ASSERT_TRUE(out.is_open());
            out << Str<T>("Hello world!");
            out.close();

            std::ifstream in(Str<T>("temp.txt"));
            ASSERT_TRUE(in.is_open());

            BaseString<T> str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            ASSERT_FALSE(str.isEmpty());
            EXPECT_TRUE(str.isDynamic());
            EXPECT_EQ(Str<T>("Hello world!"), str);
            EXPECT_EQ(12, str.size());
            in.close();
            std::filesystem::remove(Str<T>("temp.txt"));
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Modifications_RangeBasedFor)
{
    auto test = [this]<class T>()
    {
        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            std::basic_string<T> tempStdStr;
            for (auto ch : str)
            {
                tempStdStr.push_back(ch);
            }

            EXPECT_EQ(tempStdStr, str);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Modifications_SubStr)
{
    auto test = [this]<class T>()
    {
        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            str.subStr(6);
            EXPECT_EQ(Str<T>("world!"), str);
            EXPECT_EQ(6, str.size());
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            str.subStr(0, 5);
            EXPECT_EQ(Str<T>("Hello"), str);
            EXPECT_EQ(5, str.size());
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            str.subStr(2, 5);
            EXPECT_EQ(Str<T>("llo"), str);
            EXPECT_EQ(3, str.size());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Find)
{
    auto test = [this]<class T>()
    {
        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            const auto* found = str.find(Str<T>(" "));
            ASSERT_TRUE(found);
            EXPECT_EQ(' ', *found);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            const auto* found = str.find(std::basic_string<T>(Str<T>(" ")));
            ASSERT_TRUE(found);
            EXPECT_EQ(' ', *found);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            const auto* found = str.find(BaseString<T>::Intern(Str<T>(" ")));
            ASSERT_TRUE(found);
            EXPECT_EQ(' ', *found);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you, world?"));
            const auto strings = str.findAll(Str<T>("world"));
            ASSERT_FALSE(strings.empty());
            for (const auto& string : strings)
            {
                EXPECT_EQ(Str<T>("world"), std::basic_string_view<T>(string, 5));
            }
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, FindIgnoreCase)
{
    auto test = [this]<class T>()
    {
        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            const auto* found = str.findIgnoreCase(Str<T>("wor"));
            ASSERT_TRUE(found);
            EXPECT_EQ('W', *found);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            const auto* found = str.findIgnoreCase(std::basic_string<T>(Str<T>(" ")));
            ASSERT_TRUE(found);
            EXPECT_EQ(' ', *found);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            const auto* found = str.findIgnoreCase(BaseString<T>::Intern(Str<T>(" ")));
            ASSERT_TRUE(found);
            EXPECT_EQ(' ', *found);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you, wOrLd?"));
            const auto strings = str.findAllIgnoreCase(Str<T>("world"));
            ASSERT_FALSE(strings.empty());
        }
    };

    int i[4] = { 1, 2, 3, 4 };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Cmp)
{
    auto test = [this]<class T>()
    {
        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            EXPECT_TRUE(str.compare(Str<T>("hello world!"), true) == Comparison::Equal);
            EXPECT_TRUE(str.compare(Str<T>("hello world"), true) == Comparison::Less);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("AAA"));
            EXPECT_TRUE(str.compare(Str<T>("bbb"), true) == Comparison::Less);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Trim)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("  MyLogin"));
            str.trimStart(' ');
            EXPECT_EQ(Str<T>("MyLogin"), str);
            EXPECT_EQ(7, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("__MyLogin"));
            str.trimStart(' ');
            EXPECT_EQ(Str<T>("__MyLogin"), str);
            EXPECT_EQ(9, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("MyLogin  "));
            str.trimEnd(' ');
            EXPECT_EQ(Str<T>("MyLogin"), str);
            EXPECT_EQ(7, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("MyLogin__"));
            str.trimEnd(' ');
            EXPECT_EQ(Str<T>("MyLogin__"), str);
            EXPECT_EQ(9, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("MyLogin  "));
            str.trim(' ');
            EXPECT_EQ(Str<T>("MyLogin"), str);
            EXPECT_EQ(7, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("  MyLogin  "));
            str.trim(' ');
            EXPECT_EQ(Str<T>("MyLogin"), str);
            EXPECT_EQ(7, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("MyLogin__"));
            str.trim(' ');
            EXPECT_EQ(Str<T>("MyLogin__"), str);
            EXPECT_EQ(9, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("MyLogin;"));
            str.trim(';');
            EXPECT_EQ(Str<T>("MyLogin"), str);
            EXPECT_EQ(7, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("   "));
            str.trim(' ');
            EXPECT_EQ(Str<T>(""), str);
            EXPECT_EQ(0, str.size());
            EXPECT_NE(0, str.capacity());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("   "));
            str.trimEnd(' ');
            EXPECT_EQ(Str<T>(""), str);
            EXPECT_EQ(0, str.size());
            EXPECT_NE(0, str.capacity());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, ToLower)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            str.toLowerCase();
            EXPECT_EQ(Str<T>("hello world!"), str);
            EXPECT_EQ(12, str.size());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, ToUpper)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            str.toUpperCase();
            EXPECT_EQ(Str<T>("HELLO WORLD!"), str);
            EXPECT_EQ(12, str.size());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, PushBack)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.push_back('!');
            EXPECT_EQ(Str<T>("Hello World!"), str);
            EXPECT_EQ(12, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            std::basic_string<T> text = Str<T>(
                "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum");
            str.push_back(text.data());
            EXPECT_EQ(
                Str<T>(
                    "Hello WorldLorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum"),
                str);
            EXPECT_EQ(11 + text.size(), str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.push_back(std::basic_string<T>(Str<T>("!!!")));
            EXPECT_EQ(Str<T>("Hello World!!!"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.push_back(std::basic_string<T>(Str<T>("!!!")));
            EXPECT_EQ(Str<T>("Hello World!!!"), str);
            EXPECT_EQ(14, str.size());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, PushFront)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.push_front('!');
            EXPECT_EQ(Str<T>("!Hello World"), str);
            EXPECT_EQ(12, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            std::basic_string<T> text = Str<T>(
                "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum");
            str.push_front(text.data());
            EXPECT_EQ(
                Str<T>(
                    "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem IpsumHello World"),
                str);
            EXPECT_EQ(11 + text.size(), str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.push_front(std::basic_string<T>(Str<T>("!!!")));
            EXPECT_EQ(Str<T>("!!!Hello World"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.push_front(std::basic_string<T>(Str<T>("!!!")));
            EXPECT_EQ(Str<T>("!!!Hello World"), str);
            EXPECT_EQ(14, str.size());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Insert)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.insert(0, Str<T>("!!!"));
            EXPECT_EQ(Str<T>("!!!Hello World"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.insert(0, Str<T>("!!!"));
            EXPECT_EQ(Str<T>("!!!Hello World"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.insert(str.size(), Str<T>("!!!"));
            EXPECT_EQ(Str<T>("Hello World!!!"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.insert(str.size(), Str<T>("!!!"));
            EXPECT_EQ(Str<T>("Hello World!!!"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.insert(5, Str<T>("!!!"));
            EXPECT_EQ(Str<T>("Hello!!! World"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.insert(5, Str<T>("!!!"));
            EXPECT_EQ(Str<T>("Hello!!! World"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.insert(str.begin(), Str<T>("!!!"));
            EXPECT_EQ(Str<T>("!!!Hello World"), str);
            EXPECT_EQ(14, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World"));
            str.insert(str.begin(), Str<T>("!!!"));
            EXPECT_EQ(Str<T>("!!!Hello World"), str);
            EXPECT_EQ(14, str.size());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, PopBack)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            EXPECT_EQ(12, str.size());

            str.pop_back();

            EXPECT_EQ(Str<T>("Hello World"), str);
            EXPECT_EQ(11, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            EXPECT_EQ(12, str.size());

            str.pop_back();

            EXPECT_EQ(Str<T>("Hello World"), str);
            EXPECT_EQ(11, str.size());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, PopFront)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            EXPECT_EQ(12, str.size());
            std::basic_string<T> s;
            str.pop_front();

            EXPECT_EQ(Str<T>("ello World!"), str);
            EXPECT_EQ(11, str.size());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            EXPECT_EQ(12, str.size());
            std::basic_string<T> s;
            str.pop_front();

            EXPECT_EQ(Str<T>("ello World!"), str);
            EXPECT_EQ(11, str.size());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, ShrinkToFit)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            EXPECT_EQ(12, str.size());
            EXPECT_EQ(12 + 1, str.capacity());

            str.reserve(100);
            EXPECT_EQ(12, str.size());
            EXPECT_EQ(100, str.capacity());

            str.shrink_to_fit();
            EXPECT_EQ(12, str.size());
            EXPECT_EQ(12 + 1, str.capacity());
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World!"));
            EXPECT_EQ(12, str.size());
            EXPECT_EQ(12 + 1, str.capacity());

            str.reserve(100);
            EXPECT_EQ(12, str.size());
            EXPECT_EQ(100, str.capacity());

            str.shrink_to_fit();
            EXPECT_EQ(12, str.size());
            EXPECT_EQ(12 + 1, str.capacity());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Replace)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World! Hello World!"));
            std::basic_string<T> t1 = Str<T>("llo");
            std::basic_string<T> t2 = Str<T>("LLO___LLO");
            str.replaceFirst(t1.data(), t2.data());
            EXPECT_EQ(Str<T>("HeLLO___LLO World! Hello World!"), str);
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello World! Hello World!"));
            std::basic_string<T> t1 = Str<T>("o");
            std::basic_string<T> t2 = Str<T>("!o!");
            str.replaceAll(t1.data(), t2.data());
            EXPECT_EQ(Str<T>("Hell!o! W!o!rld! Hell!o! W!o!rld!"), str);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Copy)
{
    auto test = [this]<class T>()
    {
        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            T arr[128]{};
            str.copyTo(arr, str.size());
            EXPECT_EQ(str, arr);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, AdvanceWorkFlow)
{
    auto test = [this]<class T>()
    {
        auto str = BaseString<T>::Intern(Str<T>("Hello World!"));

        {
            auto tokens = str.split(Str<T>(" "));
            ASSERT_EQ(2, tokens.size());
            EXPECT_EQ(Str<T>("Hello"), tokens[0]);
            EXPECT_EQ(Str<T>("World!"), tokens[1]);
        }

        str.toLowerCase();
        ASSERT_EQ(Str<T>("hello world!"), str);
        ASSERT_EQ(str, Str<T>("hello world!"));
        ASSERT_EQ(std::basic_string<T>(Str<T>("hello world!")), str);
        ASSERT_EQ(str, std::basic_string<T>(Str<T>("hello world!")));

        str.toUpperCase();
        ASSERT_EQ(Str<T>("HELLO WORLD!"), str);
        ASSERT_EQ(str, Str<T>("HELLO WORLD!"));
        ASSERT_EQ(std::basic_string<T>(Str<T>("HELLO WORLD!")), str);
        ASSERT_EQ(str, std::basic_string<T>(Str<T>("HELLO WORLD!")));

        std::basic_string<T> t1 = Str<T>("!");
        std::basic_string<T> t2 = Str<T>("???");
        str.replaceAll(t1.data(), t2.data());
        ASSERT_EQ(Str<T>("HELLO WORLD???"), str);
        ASSERT_EQ(str, Str<T>("HELLO WORLD???"));
        ASSERT_EQ(std::basic_string<T>(Str<T>("HELLO WORLD???")), str);
        ASSERT_EQ(str, std::basic_string<T>(Str<T>("HELLO WORLD???")));
        ASSERT_TRUE(BaseString<T>::Toolset::Cmp(str.c_str(), Str<T>("HELLO WORLD???")) == Comparison::Equal);

        ASSERT_TRUE(str.compare(Str<T>("AAA"), true) == Comparison::Greater);
        ASSERT_TRUE(str.compare(Str<T>("AAA")) == Comparison::Greater);
        ASSERT_TRUE(str.compare(Str<T>("aaa")) == Comparison::Less);

        str.trim('?');
        ASSERT_EQ(str, Str<T>("HELLO WORLD"));
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, AdvanceWorkFlow2)
{
    auto test = [this]<class T>()
    {
        BaseString<T> str(128);
        ASSERT_TRUE(str.isEmpty());
        ASSERT_TRUE(!str);
        if (!str)
        {
            str.push_back(Str<T>("Hello"));
            ASSERT_FALSE(str.isEmpty());
            str.push_back(Str<T>("World"));
            ASSERT_FALSE(str.isEmpty());
            ASSERT_EQ(Str<T>("HelloWorld"), str);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, AdvanceWorkFlow3)
{
    auto test = [this]<class T>()
    {
        BaseString<T> str;
        str.resize(5);
        if (!str)
        {
            str.push_back(Str<T>("Hello"));
            ASSERT_FALSE(str.isEmpty());
            str.push_back(Str<T>("World"));
            ASSERT_FALSE(str.isEmpty());
            ASSERT_EQ(Str<T>("     HelloWorld"), str);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Format)
{
    auto test = [this]<class T>()
    {
        std::basic_string<T> jenny = Str<T>("Jenny");
        std::basic_string<T> coffee = Str<T>("coffee");
        const auto str = BaseString<T>::Format(Str<T>(R"(Hello {}! I have {}$. If u want we can go to {}.)"), jenny.data(), 300, coffee.data());
        EXPECT_EQ(Str<T>("Hello Jenny! I have 300$. If u want we can go to coffee."), str);
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, LinesCount)
{
    auto test = [this]<class T>()
    {
        const auto str = BaseString<T>::Intern(Str<T>(R"(Hello
World!
How are you?)"));
        EXPECT_EQ(3, BaseString<T>::GetLinesCountInText(str.c_str(), str.c_str() + str.size()));
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, IterateOverLines)
{
    auto test = [this]<class T>()
    {
        const auto str = BaseString<T>::Intern(Str<T>("Hello\nWorld!\nHow are you?"));

        std::vector<BaseString<T>> lines = { Str<T>("Hello"), Str<T>("World!"), Str<T>("How are you?") };

        std::vector<bool> linesValidator;
        int i = 0;
        str.forEachByLine(
            [&](auto arg)
            {
                linesValidator.push_back(arg == lines[i++]);
                return true;
            },
            BaseString<T>::LineSeparator::LF);

        ASSERT_EQ(lines.size(), linesValidator.size());

        for (auto isValid : linesValidator)
        {
            EXPECT_TRUE(isValid);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Erase)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            str.erase(5);
            EXPECT_EQ(Str<T>("Helloworld!"), str);
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            str.erase(5, 7);
            EXPECT_EQ(Str<T>("Hellorld!"), str);
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            str.erase(str.begin() + 5);
            EXPECT_EQ(Str<T>("Helloworld!"), str);
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
            str.erase(str.begin() + 5, str.begin() + 7);
            EXPECT_EQ(Str<T>("Hellorld!"), str);
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Misc)
{
    auto test = [this]<class T>()
    {
        EXPECT_TRUE(BaseString<T>::IsSpace(' '));
        EXPECT_TRUE(BaseString<T>::IsSpace('\n'));
        EXPECT_TRUE(BaseString<T>::IsSpace('\t'));
        EXPECT_TRUE(BaseString<T>::IsSpace('\r'));
        EXPECT_FALSE(BaseString<T>::IsSpace('a'));

        EXPECT_TRUE(BaseString<T>::IsContainChar('a', Str<T>("abcdef")));
        EXPECT_TRUE(BaseString<T>::IsContainChar('c', Str<T>("abcdef")));
        EXPECT_TRUE(BaseString<T>::IsContainChar('f', Str<T>("abcdef")));
        EXPECT_FALSE(BaseString<T>::IsContainChar('z', Str<T>("abcdef")));
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, Adding)
{
    auto test = [this]<class T>()
    {
        std::basic_string<T> helloWorld = Str<T>("Hello world!");
        std::basic_string<T> hello = Str<T>("Hello ");
        EXPECT_EQ(helloWorld.data(), hello.data() + BaseString<T>(Str<T>("world!")));
        EXPECT_EQ(helloWorld.data(), hello.data() + BaseString<T>::Intern(Str<T>("world!")));
        EXPECT_EQ(helloWorld.data(), hello.data() + BaseString<T>::Intern(Str<T>("world")) + Str<T>("!"));
        EXPECT_EQ(helloWorld.data(), hello.data() + BaseString<T>::Intern(Str<T>("world")) + BaseString<T>::Intern(Str<T>("!")));
        EXPECT_EQ(helloWorld.data(), BaseString<T>(hello.data()) + Str<T>("world!"));
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, SimpleCopy)
{
    auto test = [this]<class T>()
    {
        BaseString<T> str = Str<T>("Hello world!");
        const auto baseCapacity = str.capacity();
        str.shrink_to_fit();
        const auto shrinkedCapacity = str.capacity();
        EXPECT_LE(shrinkedCapacity, baseCapacity);

        BaseString<T> copyStr = Str<T>("Smth");
        copyStr = str;
        EXPECT_EQ(shrinkedCapacity, str.capacity());
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, WorkingWithStdFilesystemPath)
{
    auto test = [this]<class T>()
    {
        {
            auto str = BaseString<T>(Str<T>("Path: "));
            str += BaseString<T>::MakeFrom(std::filesystem::current_path());
        }

        {
            auto str = BaseString<T>(Str<T>("Path: "));
            auto path = std::filesystem::current_path();
            str += BaseString<T>::MakeFrom(path);
        }

        {
            auto str = BaseString<T>(Str<T>("Path: "));
            const auto path = std::filesystem::current_path();
            str += BaseString<T>::MakeFrom(path);
        }

        {
            auto path = std::filesystem::current_path();
            auto str = BaseString<T>(Str<T>("Path: ")) + BaseString<T>::MakeFrom(path);
        }

        {
            auto str = BaseString<T>(Str<T>("Path: ")) + BaseString<T>::MakeFrom(std::filesystem::current_path());
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, ForEachByLine)
{
    auto test = [this]<class T>()
    {
        using namespace Core;

        BaseString<T> str = Str<T>("Hello\nWorld\n!");
        std::vector<BaseString<T>> tokens;
        str.forEachByLine(
            [&tokens](auto str)
            {
                tokens.emplace_back(std::move(str));
            });

        ASSERT_EQ(3, tokens.size());
        EXPECT_EQ(Str<T>("Hello"), tokens[0]);
        EXPECT_EQ(Str<T>("World"), tokens[1]);
        EXPECT_EQ(Str<T>("!"), tokens[2]);
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, FindNextLine)
{
    auto test = [this]<class T>()
    {
        using namespace Core;

        BaseString<T> str = Str<T>("Hello\nWorld\n!");
        std::vector<BaseString<T>> tokens = { Str<T>("Hello"), Str<T>("World"), Str<T>("!") };

        const auto* ptr = str.c_str();

        const auto firstLineStr = BaseString<T>(ptr, tokens[0].size());
        ASSERT_TRUE(firstLineStr);
        ASSERT_EQ(Str<T>("Hello"), firstLineStr);

        const auto secondLine = BaseString<T>::FindNextLine(ptr);
        ASSERT_TRUE(secondLine);
        const auto secondLineStr = BaseString<T>(secondLine, tokens[1].size());
        ASSERT_EQ(Str<T>("World"), secondLineStr);
        ptr = secondLine;

        const auto thirdLine = BaseString<T>::FindNextLine(ptr);
        ASSERT_TRUE(thirdLine);
        const auto thirdLineStr = BaseString<T>(thirdLine, tokens[2].size());
        ASSERT_EQ(Str<T>("!"), thirdLineStr);
        ptr = thirdLine;

        EXPECT_EQ(nullptr, BaseString<T>::FindNextLine(ptr));
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, ReverseStrStr)
{
    auto test = [this]<class T>()
    {
        using namespace Core;

        const auto lenHello = BaseString<T>(Str<T>("Hello")).length();

        {
            BaseString<T> str = Str<T>("HelloHello");
            EXPECT_EQ(str.c_str() + lenHello, str.reverseFind(Str<T>("Hello")));
        }

        {
            BaseString<T> str = Str<T>("HelloHello");
            EXPECT_EQ(str.c_str(), str.reverseFind(Str<T>("Hello"), 0, 1));
        }

        {
            BaseString<T> str = Str<T>("");
            EXPECT_EQ(nullptr, str.reverseFind(Str<T>("Hello"), 0, 1));
        }
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, FindPrevLine)
{
    auto test = [this]<class T>()
    {
        using namespace Core;

        BaseString<T> str = Str<T>("Hello\nWorld\n!");
        std::vector<BaseString<T>> tokens = { Str<T>("Hello"), Str<T>("World"), Str<T>("!") };

        const auto* ptr = BaseString<T>::FindPrevLine(str.c_str());
        ASSERT_TRUE(ptr);
        const auto thirdLine = BaseString<T>(ptr, tokens[2].size());
        EXPECT_EQ(Str<T>("!"), thirdLine);

        ptr = BaseString<T>::FindPrevLine(str.c_str(), ptr - 1);
        ASSERT_TRUE(ptr);
        const auto secondLine = BaseString<T>(ptr, tokens[1].size());
        EXPECT_EQ(Str<T>("World"), secondLine);

        ptr = BaseString<T>::FindPrevLine(str.c_str(), ptr - 1);
        ASSERT_TRUE(ptr);
        const auto firstLine = BaseString<T>(ptr, tokens[0].size());
        EXPECT_EQ(Str<T>("Hello"), firstLine);
    };

    test.template operator()<char>();
    test.template operator()<wchar_t>();
}

TEST_F(StringTestF, RegexMatch)
{
    auto test = [this]<class T>()
    {
        {
            // check for pascal case
            auto str = BaseString<T>::Intern(Str<T>("RegEx"));
            EXPECT_TRUE(str.regexMatch(Str<T>("^([A-Z][a-z0-9]+)+$")));
        }

        {
            auto str = BaseString<T>::Intern(Str<T>("RegEx\n\rHello"));
            EXPECT_TRUE(str.regexMatch(Str<T>("^(\\w+\\s*)+$")));
        }
    };

    test.template operator()<char>();
    // test.template operator()<wchar_t>(); - regex only for char
}

TEST_F(StringTestF, RegexFind)
{
    auto test = [this]<class T>()
    {
        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            const auto match = str.regexFind(Str<T>(" \\w+"));
            ASSERT_TRUE(match.isMatched());
            EXPECT_EQ(Str<T>(" world"), match.convertBasedOn(str));
        }

        {
            BaseString<T> str = BaseString<T>::Intern(Str<T>("Hello world!"));
            const auto match = str.regexFind(Str<T>(" \\w+"), 3);
            ASSERT_TRUE(match.isMatched());
            EXPECT_EQ(Str<T>(" world"), match.convertBasedOn(str));
        }

        {
            BaseString<T> str = Str<T>("Hello world!");
            const auto match = str.regexFind(Str<T>(" \\w+"));
            ASSERT_TRUE(match.isMatched());
            EXPECT_EQ(Str<T>(" world"), match.convertBasedOn(str));
        }

        {
            BaseString<T> str = Str<T>("Hello world!");
            const auto match = str.regexFind(Str<T>(" \\w+"), 3);
            ASSERT_TRUE(match.isMatched());
            EXPECT_EQ(Str<T>(" world"), match.convertBasedOn(str));
        }

        {
            BaseString<T> str = Str<T>("Hello world!");
            const auto match = str.regexFind(Str<T>(" \\w+"));
            EXPECT_EQ(Str<T>(" world"), match.convertBasedOn(str));
        }

        {
            BaseString<T> str = Str<T>("Hello world!");
            const auto match = str.regexFind(Str<T>(" \\w+"), 3);
            EXPECT_EQ(Str<T>(" world"), match.convertBasedOn(str));
        }
    };

    test.template operator()<char>();
    // test.template operator()<wchar_t>(); - regex only for char
}

TEST_F(StringTestF, RegexFindAll)
{
    auto test = [this]<class T>()
    {
        auto str = BaseString<T>::Intern(Str<T>("How are you, Jim?"));
        auto vec = str.regexFindAll(Str<T>("\\w+"));
        ASSERT_EQ(4, vec.size());
        EXPECT_EQ(Str<T>("How"), vec[0].convertBasedOn(str));
        EXPECT_EQ(Str<T>("are"), vec[1].convertBasedOn(str));
        EXPECT_EQ(Str<T>("you"), vec[2].convertBasedOn(str));
        EXPECT_EQ(Str<T>("Jim"), vec[3].convertBasedOn(str));
    };

    test.template operator()<char>();
    // test.template operator()<wchar_t>(); - regex only for char
}

TEST_F(StringTestF, RegexIterate)
{
    auto test = [this]<class T>()
    {
        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you?"));
            BaseString<T> buffer;
            str.regexIterate("\\w+",
                             [&buffer, &str](const RegexMatch::MatchedData& match)
                             {
                                 buffer.push_back(match.convertBasedOn(str));
                                 return true;
                             });
            EXPECT_EQ(Str<T>("HelloworldHowareyou"), buffer);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you?"));
            BaseString<T> buffer;
            str.regexIterate(std::basic_string<char>("\\w+"),
                             [&buffer, &str](const RegexMatch::MatchedData& match)
                             {
                                 buffer.push_back(match.convertBasedOn(str));
                                 return true;
                             });
            EXPECT_EQ(Str<T>("HelloworldHowareyou"), buffer);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you?"));
            BaseString<T> buffer;
            std::basic_string<char> expr("\\w+");
            str.regexIterate(expr.c_str(),
                             [&buffer, &str](const RegexMatch::MatchedData& match)
                             {
                                 buffer.push_back(match.convertBasedOn(str));
                                 return true;
                             });
            EXPECT_EQ(Str<T>("HelloworldHowareyou"), buffer);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you?"));
            BaseString<T> buffer;
            std::basic_string_view<char> expr("\\w+");
            str.regexIterate(expr.data(),
                             [&buffer, &str](const RegexMatch::MatchedData& match)
                             {
                                 buffer.push_back(match.convertBasedOn(str));
                                 return true;
                             });
            EXPECT_EQ(Str<T>("HelloworldHowareyou"), buffer);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you?"));
            BaseString<T> buffer;
            const auto expr = BaseString<char>::Intern("\\w+");
            str.regexIterate(expr,
                             [&buffer, &str](const RegexMatch::MatchedData& match)
                             {
                                 buffer.push_back(match.convertBasedOn(str));
                                 return true;
                             });
            EXPECT_EQ(Str<T>("HelloworldHowareyou"), buffer);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you?"));
            BaseString<T> buffer;
            str.regexIterate(BaseString<char>::Intern("\\w+"),
                             [&buffer, &str](const RegexMatch::MatchedData& match)
                             {
                                 buffer.push_back(match.convertBasedOn(str));
                                 return true;
                             });
            EXPECT_EQ(Str<T>("HelloworldHowareyou"), buffer);
        }

        {
            const auto str = BaseString<T>::Intern(Str<T>("Hello world! How are you?"));
            BaseString<T> buffer;
            str.regexIterate(std::basic_string_view<char>("\\w+"),
                             [&buffer, &str](const RegexMatch::MatchedData& match)
                             {
                                 buffer.push_back(match.convertBasedOn(str));
                                 return true;
                             });
            EXPECT_EQ(Str<T>("HelloworldHowareyou"), buffer);
        }
    };

    test.template operator()<char>();
    // test.template operator()<wchar_t>(); - regex only for char
}

TEST_F(StringTestF, RegexReplace)
{
    auto test = [this]<class T>()
    {
        auto str = BaseString<T>::Intern(Str<T>("Hello world!"));
        EXPECT_FALSE(str.regexReplace(Str<T>("\\?"), Str<T>("")));
        EXPECT_EQ(Str<T>("Hello world!"), str);
        EXPECT_TRUE(str.regexReplace(Str<T>(" "), Str<T>("_")));
        EXPECT_EQ(Str<T>("Hello_world!"), str);
    };

    test.template operator()<char>();
    // test.template operator()<wchar_t>(); - regex only for char
}