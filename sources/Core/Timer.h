//  MIT License
//
//  Copyright (c) 2019-2025 Valerii Koniushenko
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include <chrono>
#include <codecvt>
#include <functional>

namespace Core
{

    template<class Type>
    class Stopwatch final
    {
    private:
        using clock = std::chrono::system_clock;
        using duration = std::chrono::duration<Type>;

    public:
        void start() { _startTime = clock::now(); }
        [[nodiscard]] Type stop() { return duration(clock::now() - _startTime).count(); }

    private:
        clock::time_point _startTime;
    };

    using FStopwatch = Stopwatch<float>;
    using DStopwatch = Stopwatch<double>;

    class Repeater final
    {
    public:
        using CallbackT = std::function<void(double)>;

    public:
        explicit Repeater(double value = 0) { setRepeatTime(value); };
        ~Repeater() = default;

        [[nodiscard]] double getRepeatTime() const noexcept { return _repeatTime; }
        void setRepeatTime(double value) { _repeatTime = value; }

        /**
         * @brief will call the callback and pass one arg: delta time
         */
        void setCallback(CallbackT&& callback) { _callback = std::forward<CallbackT>(callback); };
        void reset()
        {
            _callback = nullptr;
            _startTime.reset();
            _lastCall = {};
        }
        void startOrUpdate();
        [[nodiscard]] double getTimeGap() const noexcept;

    private:
        CallbackT _callback;
        double _repeatTime = 1.0;
        std::chrono::system_clock::time_point _lastCall;
        std::optional<std::chrono::system_clock::time_point> _startTime;
    };

} // namespace Core