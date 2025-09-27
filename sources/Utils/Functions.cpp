// MIT License
//
// Copyright (c) 2018-2025 Valerii Koniushenko
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

#include "Functions.h"

namespace Utils
{

    bool IsReadable(const std::filesystem::path& p) noexcept
    {
        using namespace std::filesystem;

        std::error_code ec; // For noexcept overload usage.
        auto perms = status(p, ec).permissions();
        if ((perms & perms::owner_read) != perms::none && (perms & perms::group_read) != perms::none && (perms & perms::others_read) != perms::none)
        {
            return true;
        }
        return false;
    }

    std::vector<char> GetFileContent(const std::filesystem::path& path)
    {
        std::vector<char> out;

        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Impossible to open a file: " + path.generic_string());
        }

        const auto size = file.tellg();
        out.resize(size);
        file.seekg(0, std::ios_base::beg);
        file.read(out.data(), size);
        file.close();

        return out;
    }

} // namespace Utils
