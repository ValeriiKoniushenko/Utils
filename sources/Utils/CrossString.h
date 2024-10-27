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

#pragma once

#ifndef _MSC_VER

    #include <stdarg.h>
    #include <string.h>

inline char* strtok_s(char* s, const char* delim, char** context)
{
    return strtok_r(s, delim, context);
}

inline int _wtoi(const wchar_t* str)
{
    return static_cast<int>(wcstol(str, 0, 10));
}

inline float _wtof(const wchar_t* str)
{
    wchar_t* endptr = nullptr;
    return static_cast<int>(wcstof(str, &endptr));
}

inline long long _wtoll(const wchar_t* str)
{
    wchar_t* endptr = nullptr;
    return static_cast<int>(wcstoll(str, &endptr, 10));
}

inline int _snwprintf_s(wchar_t* s, std::size_t n, std::size_t, const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    auto result = vswprintf(s, n, format, args);
    va_end(args);
    return result;
}

inline wchar_t* wcstok_s(wchar_t* str, const wchar_t* delim, wchar_t** ptr)
{
    return wcstok(str, delim, ptr);
}

inline void* memcpy_s(void* dest, std::size_t destsz, const void* src, std::size_t count)
{
    return memcpy(dest, src, count);
}

#endif
