// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
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

#include "Core/Assert.h"

#include <filesystem>
#include <fstream>

namespace Utils
{
    template<class T>
    [[nodiscard]] T getTextFileContentAs(const std::filesystem::path& path)
    {
        std::ifstream in(path);
        if (!in.is_open())
        {
            in.close();
            Assert(false, ("Impossible to open a file: " + path.string()).c_str());
            return {};
        }

        T data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        in.close();
        return data;
    }

    template<class T>
    [[nodiscard]] T tryToGetTextFileContentAs(const std::filesystem::path& path)
    {
        std::ifstream in(path);
        if (!in.is_open())
        {
            in.close();
            return {};
        }

        T data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        in.close();
        return data;
    }
} // namespace Utils

/*#include "Size.h"
#include "json.hpp"

#include <string>
#include <vector>

namespace Utils
{

[[nodiscard]] std::vector<ISize2D> getAllSupportedWndSizes();

[[nodiscard]] std::vector<std::string> split(const std::string& string, char devider);
[[nodiscard]] bool isNumber(const std::string& string);
[[nodiscard]] std::string toString(const std::vector<std::string>& data, const std::string& delimiter = ", ");

}     // namespace Utils*/
