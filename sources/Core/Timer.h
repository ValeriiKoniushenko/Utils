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

#include <chrono>
#include <cmath>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>

namespace Core
{
    /**
     * @brief A utility class for measuring elapsed time.
     *
     * The Stopwatch class provides methods for starting and reading
     * a timer to measure the elapsed time. It uses a steady clock to
     * ensure monotonic time measurements. The stopwatch is templated
     * to allow customization of the type for the elapsed time's representation.
     *
     * @tparam Type The numeric type that represents the elapsed time (e.g., float, double).
     */
    template<class Type>
    class Stopwatch final
    {
    private:
        using ClockT = std::chrono::steady_clock;
        using DurationT = std::chrono::duration<Type>;

    public:
        /**
         * @brief Constructs and immediately starts the stopwatch.
         */
        Stopwatch() noexcept
            : _startTime(ClockT::now())
        {
        }

        /**
         * @brief Starts the stopwatch and records the current time as the start time.
         *
         * This method initializes or resets the start time of the stopwatch to the current time,
         * which can be later used to measure elapsed time or duration.
         */
        void start() noexcept { _startTime = ClockT::now(); }

        /**
         * @brief Calculates the elapsed time since the stopwatch was last started.
         *
         * This method determines the duration that has passed since the stopwatch was last started
         * and returns the elapsed time in the type specified by the template parameter. It does not
         * pause the stopwatch; repeated calls continue to measure from the same start time.
         *
         * @return The elapsed time since the stopwatch was started, in the specified type.
         */
        [[nodiscard]] Type stop() const noexcept
        {
            return DurationT(ClockT::now() - _startTime).count();
        }

    private:
        ClockT::time_point _startTime;
    }
    ;

    using FStopwatch = Stopwatch<float>;
    using DStopwatch = Stopwatch<double>;

    /**
     * @brief Represents a class that provides functionality to invoke a callback
     * repeatedly based on a specified time interval.
     */
    class Repeater final
    {
    public:
        using CallbackT = std::function<void(float)>;
        using ClockT = std::chrono::steady_clock;
        using DurationT = std::chrono::duration<float>;

    public:
        explicit Repeater(float value = 0) { setRepeatTime(value); }
        Repeater(const Repeater& other);
        Repeater& operator=(const Repeater& other);
        Repeater(Repeater&&) noexcept = default;
        Repeater& operator=(Repeater&&) noexcept = default;
        ~Repeater() = default;

        /**
         * @brief Retrieves the repeat time interval for the repeater.
         *
         * @return The repeat time interval in seconds.
         */
        [[nodiscard]] float getRepeatTime() const noexcept { return _repeatTime; }

        /**
         * @brief Sets the repeat time interval for the repeater.
         * @param value The repeat time in seconds. Values less than or equal to zero make the
         * callback eligible on every update.
         * @throws std::invalid_argument If @p value is NaN or infinite.
         */
        void setRepeatTime(float value)
        {
            if (!std::isfinite(value))
            {
                throw std::invalid_argument("Repeater interval must be finite");
            }
            _repeatTime = value;

            int* i = NULL;
        }

        /**
         * @brief Sets the callback invoked for an eligible update.
         *
         * The callback receives the elapsed seconds since the previous eligible update. It may
         * safely reset the repeater or replace itself. Timing state is committed before invocation,
         * so exceptions and recursive updates do not leave a stale last-call timestamp.
         *
         * @param callback The callback to install, or an empty function to clear it.
         */
        void setCallback(const CallbackT& callback)
        {
            _callback = callback ? std::make_shared<CallbackT>(callback) : nullptr;
        }

        /**
         * @brief Resets the repeater's internal state, including clearing the callback, resetting
         * the start time, and last call time.
         */
        void reset();

        /**
         * @brief Initiates the repeater or updates its state. If the repeater is not yet started,
         * it initializes the start time. If the time interval since the last call reaches the
         * repeat time, the callback function (if set) is invoked with the delta time as an
         * argument. A positive interval therefore waits for a later update, while a zero or
         * negative interval is eligible immediately with an initial delta of zero.
         */
        void startOrUpdate();

        /**
         * @brief Retrieves the duration in seconds between the start time and the last call.
         * If the repeater has not started, this method returns 0.
         *
         * @return The time gap between the start time and the last call, in seconds.
         */
        [[nodiscard]] float getTimeGap() const noexcept;

    private:
        std::shared_ptr<CallbackT> _callback;
        ClockT::time_point _lastCall;
        std::optional<ClockT::time_point> _startTime;
        float _repeatTime = 0.0;
    };
} // namespace Core
