// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#if defined(__unix__) || (defined(__MINGW32__) && !defined(_MSC_VER))

    #include <cinttypes>
    #include <cstring>
    #include <cassert>


inline int memcpy_s(void* dest, [[maybe_unused]] uint64_t destsz, const void* src, uint64_t count)
{
    assert(count <= destsz);
    memcpy(dest, src, count);
    return 0;
}

#endif