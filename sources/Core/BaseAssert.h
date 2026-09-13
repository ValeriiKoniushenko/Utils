// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include <cassert>
#include <iostream>

#ifdef _MSC_VER
// clang-format off
    #include <windows.h>
    #include <debugapi.h>
// clang-format on
#endif

#if defined(__cpp_lib_stacktrace) && !defined(DONT_USE_CPP_LIB_STACKTRACE)                         \
    && !(defined(__clang__) && defined(__GLIBCXX__)) && !defined(__GNUC__)
    #include <stacktrace>
#endif

#if defined(UTILS_DEBUG)

inline void Assert(bool condition, const char* message = nullptr)
{
    if (condition)
    {
        return;
    }

    using std::cerr;
    using std::endl;

    cerr << "Assert was got: " << endl
         << "Message: " << (message ? message : "None") << endl
    #if defined(__cpp_lib_stacktrace) && !defined(DONT_USE_CPP_LIB_STACKTRACE)                     \
        && !(defined(__clang__) && defined(__GLIBCXX__)) && !defined(__GNUC__)
         << "Stacktrace: " << endl
         << std::stacktrace::current() << endl
    #endif
        ;
    assert(condition);
}

inline bool Verify(bool condition, const char* message = nullptr)
{
    Assert(condition, message);
    return condition;
}

[[maybe_unused]] inline void Assert(const char* message)
{
    Assert(false, message);
}

#else

    #define Verify(cond, ...) (cond)
    #define Assert(cond, ...) ((void)0)

#endif