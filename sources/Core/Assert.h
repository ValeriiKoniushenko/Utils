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

#if defined(UTILS_DEBUG) || defined(NDEBUG) || defined(DEBUG)

    #include "BaseAssert.h"
    #include "String.h"

    #include <chrono>

inline void Assert(bool condition, const Core::StringAtom& message)
{
    ::Assert(condition, message.c_str());
}

inline bool Verify(bool condition, const Core::StringAtom& message)
{
    return ::Verify(condition, message.c_str());
}

[[maybe_unused]] inline void Assert(const Core::StringAtom& message)
{
    ::Assert(false, message.c_str());
}

    // ─────────────────────────────────────────────────────────────────────────────
    // AssertOnce — fires at most once per call site, no matter how many times the
    //              condition is false afterward.
    // ─────────────────────────────────────────────────────────────────────────────
    #define AssertOnce(cond, ...)                                                                  \
        do                                                                                         \
        {                                                                                          \
            static std::atomic<bool> _once_fired{ false };                                         \
            /* exchange returns the OLD value; if it was false we are first */                     \
            if (!(cond) && !_once_fired.exchange(true, std::memory_order_relaxed))                 \
            {                                                                                      \
                Assert(false, ##__VA_ARGS__);                                                      \
            }                                                                                      \
        } while (0)

    // ─────────────────────────────────────────────────────────────────────────────
    // AssertAtCount — fires only on the N-th failure of the condition.
    //                 All other failures (before or after N) are silent.
    // ─────────────────────────────────────────────────────────────────────────────
    #define AssertAtCount(cond, n, ...)                                                            \
        do                                                                                         \
        {                                                                                          \
            static std::atomic<int> _fail_count{ 0 };                                              \
            if (!(cond) && ++_fail_count == (n))                                                   \
            {                                                                                      \
                Assert(false, ##__VA_ARGS__);                                                      \
            }                                                                                      \
        } while (0)

    // ─────────────────────────────────────────────────────────────────────────────
    // AssertTimed — fires when the condition is false, but rate-limited to once
    //               per `seconds` seconds.  The first failure always fires; after
    //               that the assert is silenced until the cooldown expires.
    //
    //               Thread-safe: compare_exchange ensures only one thread wins when
    //               multiple threads cross the threshold simultaneously.
    // ─────────────────────────────────────────────────────────────────────────────
    #define AssertTimed(cond, seconds, ...)                                                        \
        do                                                                                         \
        {                                                                                          \
            using namespace std::chrono;                                                           \
            using ClockT = std::chrono::steady_clock;                                              \
                                                                                                   \
            if (!cond)                                                                             \
            {                                                                                      \
                static thread_local const auto firstFire                                           \
                    = duration_cast<milliseconds>(ClockT::now().time_since_epoch()).count();       \
                                                                                                   \
                const auto now                                                                     \
                    = duration_cast<milliseconds>(ClockT::now().time_since_epoch()).count();       \
                                                                                                   \
                if (now - firstFire >= seconds)                                                    \
                {                                                                                  \
                    Assert(false, ##__VA_ARGS__);                                                  \
                }                                                                                  \
            }                                                                                      \
        } while (0)

#else // !UTILS_DEBUG — all three are complete no-ops

    #define AssertOnce(cond, ...)        ((void)0)
    #define AssertAtCount(cond, n, ...)  ((void)0)
    #define AssertTimed(cond, secs, ...) ((void)0)

#endif
