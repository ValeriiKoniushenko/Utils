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

#include "Regex.h"

namespace Core::SPcre2
{
    BaseRegex::~BaseRegex()
    {
        Clear();
    }

    BaseRegex::String BaseRegex::GetErrorString(const BaseRegex& regex)
    {
        char buffer[256]{};
        pcre2_get_error_message(regex._errorCode, reinterpret_cast<PCRE2_UCHAR8*>(buffer), sizeof(buffer));
        if (buffer[0] != '\0')
        {
            return "Regex failed at the offset: {}. Error message: {}"_f << regex._errorOffset << buffer;
        }

        return {};
    }

    bool BaseRegex::Compile()
    {
        _regex = pcre2_compile(reinterpret_cast<PCRE2_SPTR8>(_pattern.c_str()), // The regex pattern
                               PCRE2_ZERO_TERMINATED,                           // Pattern is null-terminated
                               _options,                                        // Default options
                               &_errorCode,                                     // Error code
                               &_errorOffset,                                   // Error offset
                               nullptr                                          // Default compile context
        );

        if (_regex == nullptr)
        {
            return false;
        }

        return true;
    }
} // namespace Core::SPcre2