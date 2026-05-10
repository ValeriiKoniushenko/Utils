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

    #include <atomic>
    #include <cassert>
    #include <chrono>
    #include <iostream>

    #ifdef _MSC_VER
    // clang-format off
    #include <windows.h>
    #include <debugapi.h>
    // clang-format on
    #endif

    #if defined(__cpp_lib_stacktrace) && !defined(DONT_USE_CPP_LIB_STACKTRACE)                     \
        && !(defined(__clang__) && defined(__GLIBCXX__)) && !defined(__GNUC__)
        #include <stacktrace>
    #endif

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

    // ─────────────────────────────────────────────────────────────────────────────
    // AssertOnce - fires at most once per call site, no matter how many times the
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
    // AssertAtCount - fires only on the N-th failure of the condition.
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
    // AssertTimed - fires when the condition is false, but rate-limited to once
    //               per `seconds` seconds.  The first failure always fires; after
    //               that the assert is silenced until the cooldown expires.
    //
    //               Thread-safe: compare_exchange ensures only one thread wins when
    //               multiple threads cross the threshold simultaneously.
    // ─────────────────────────────────────────────────────────────────────────────
    #define AssertTimed(cond, seconds, ...)                                                        \
        do                                                                                         \
        {                                                                                          \
            using _Clock = std::chrono::steady_clock;                                              \
            using _Rep = _Clock::rep;                                                              \
            /* Default time_point{} is the epoch — guarantees the first failure fires */           \
            static std::atomic<_Rep> _last_fire{                                                   \
                _Clock::time_point{}.time_since_epoch().count()                                    \
            };                                                                                     \
            if (!(cond))                                                                           \
            {                                                                                      \
                const auto _now = _Clock::now();                                                   \
                _Rep _prev = _last_fire.load(std::memory_order_relaxed);                           \
                const auto _prev_tp = _Clock::time_point{ _Clock::duration{ _prev } };             \
                const double _elapsed = std::chrono::duration<double>(_now - _prev_tp).count();    \
                /* CAS: only the thread that successfully swaps the timestamp fires */             \
                if (_elapsed >= static_cast<double>(seconds)                                       \
                    && _last_fire.compare_exchange_strong(_prev, _now.time_since_epoch().count(),  \
                                                          std::memory_order_relaxed))              \
                {                                                                                  \
                    Assert(false, ##__VA_ARGS__);                                                  \
                }                                                                                  \
            }                                                                                      \
        } while (0)

inline bool Verify(bool condition, const char* message = nullptr)
{
    Assert(condition, message);
    return condition;
}

[[maybe_unused]] inline void Assert(const char* message)
{
    Assert(false, message);
}

#else // !UTILS_DEBUG — all three are complete no-ops

    #define AssertOnce(cond, ...)        ((void)0)
    #define AssertAtCount(cond, n, ...)  ((void)0)
    #define AssertTimed(cond, secs, ...) ((void)0)
    #define Verify(cond, ...)            (cond)
    #define Assert(cond, ...)            ((void)0)

#endif