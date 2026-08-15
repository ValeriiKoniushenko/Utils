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
#include <functional>

namespace Core
{
    /**
     * @brief A utility class for measuring elapsed time.
     *
     * The Stopwatch class provides methods for starting and stopping
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
         * @brief Starts the stopwatch and records the current time as the start time.
         *
         * This method initializes or resets the start time of the stopwatch to the current time,
         * which can be later used to measure elapsed time or duration.
         */
        void start() { _startTime = ClockT::now(); }

        /**
         * @brief Stops the stopwatch and calculates the elapsed time since it started.
         *
         * This method determines the duration that has passed since the stopwatch was last started
         * and returns the elapsed time in the type specified by the template parameter.
         *
         * @return The elapsed time since the stopwatch was started, in the specified type.
         */
        [[nodiscard]] Type stop() { return DurationT(ClockT::now() - _startTime).count(); }

    private:
        ClockT::time_point _startTime;
    };

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
        explicit Repeater(float value = 0) { setRepeatTime(value); };
        Repeater(const Repeater&) = default;
        Repeater& operator=(const Repeater&) = default;
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
         * @param value The repeat time in seconds.
         */
        void setRepeatTime(float value) { _repeatTime = value; }

        /**
         * @brief will call the callback and pass one arg: delta time
         */
        void setCallback(const CallbackT& callback) { _callback = callback; }

        /**
         * @brief Resets the repeater's internal state, including clearing the callback, resetting
         * the start time, and last call time.
         */
        void reset();

        /**
         * @brief Initiates the repeater or updates its state. If the repeater is not yet started,
         * it initializes the start time. If the time interval since the last call exceeds the
         * repeat time, the callback function (if set) is invoked with the delta time as an
         * argument.
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
        CallbackT _callback;
        ClockT::time_point _lastCall;
        std::optional<ClockT::time_point> _startTime;
        float _repeatTime = 1.0;
    };
} // namespace Core
