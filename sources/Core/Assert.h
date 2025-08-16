// MIT License
//
// Copyright (c) 2023-2024 Valerii Koniushenko
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

#include <cassert>
#include <iostream>

#ifdef _MSC_VER
    #include <debugapi.h>
    #include <windows.h>
#endif

#if defined(__cpp_lib_stacktrace) && !defined(DONT_USE_CPP_LIB_STACKTRACE)
    #include <stacktrace>
#endif

#ifdef UTILS_DEBUG

inline void Assert(const bool condition, const char* message = nullptr)
{
    if (condition)
    {
        return;
    }

    using std::cerr;
    using std::endl;

    cerr << "Assert was got: " << endl
         << "Message: " << (message ? message : "None") << endl
    #if defined(__cpp_lib_stacktrace) && !defined(DONT_USE_CPP_LIB_STACKTRACE)
         << "Stacktrace: " << endl
         << std::stacktrace::current() << endl
    #endif
        ;
    assert(condition);
}

inline bool Verify(const bool condition, const char* message = nullptr)
{
    Assert(condition, message);
    return condition;
}

[[maybe_unused]] inline void Assert(const char* message = nullptr)
{
    Assert(false, message);
}

#else

    #define Verify(cond, ...) (cond)
    #define Assert(cond, ...) ((void)0)

#endif
