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

#pragma once

#include <cstdint>

namespace Core
{
    struct DefaultPolicy_IntrusivePtrRefCounter
    {
        using CounterT = uint32_t;
    };

    template<class T>
    concept IsPolicy = requires() {
        { T::CounterT };
    };

    template<class T, IsPolicy Policy = DefaultPolicy_IntrusivePtrRefCounter>
    class IntrusivePtrRefCounter
    {
    public:
        using ValueT = T;
        using CounterT = typename Policy::CounterT;

    public:
        IntrusivePtrRefCounter() noexcept
            : _refCount(0)
        {
        }

        IntrusivePtrRefCounter(const IntrusivePtrRefCounter&) noexcept
            : _refCount(0)
        {
        }

        IntrusivePtrRefCounter& operator=(const IntrusivePtrRefCounter&) noexcept { return *this; }

        [[nodiscard]] uint32_t getRefCount() const noexcept { return _refCount; }

    private:
        CounterT _refCount = 0;
    };

} // namespace Core