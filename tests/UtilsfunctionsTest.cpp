
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

    auto&& fileContent = Utils::getTextFileContentAs<std::string>(correctPath);
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

    auto&& fileContent = Utils::getTextFileContentAs<std::vector<unsigned char>>(correctPath);
    EXPECT_TRUE(memcmp(text, fileContent.data(), textLen) == 0);

    std::filesystem::remove(correctPath);
}
