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

#include "Utils/Functions.h"

#include <gtest/gtest.h>
#include <string>

TEST(UtilsTests, ReadFileContentToStdString)
{
    const char* text = R"(
Lorem Ipsum is simply dummy text of the printing and typesetting industry.
Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book.
    It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages,
and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.
)";

    const std::filesystem::path correctPath = "testFile.txt";

    std::ofstream testFile(correctPath);
    ASSERT_TRUE(testFile.is_open());
    testFile.write(text, strlen(text));
    testFile.close();

    auto&& fileContent = Utils::GetTextFileContentAs<std::string>(correctPath);
    EXPECT_STREQ(text, fileContent.c_str());

    std::filesystem::remove(correctPath);
}

TEST(UtilsTests, ReadFileContentToStdVector)
{
    const char* text = R"(
Lorem Ipsum is simply dummy text of the printing and typesetting industry.
Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book.
    It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages,
and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.
)";
    const auto textLen = strlen(text) * sizeof(unsigned char);

    const std::filesystem::path correctPath = "testFile.txt";

    std::ofstream testFile(correctPath);
    ASSERT_TRUE(testFile.is_open());
    testFile.write(text, textLen);
    testFile.close();

    auto&& fileContent = Utils::GetTextFileContentAs<std::vector<unsigned char>>(correctPath);
    EXPECT_TRUE(memcmp(text, fileContent.data(), textLen) == 0);

    std::filesystem::remove(correctPath);
}
