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

#include "Timer.h"

#include <utility>

namespace Core
{

    Repeater::Repeater(const Repeater& other)
        : _callback(other._callback ? std::make_shared<CallbackT>(*other._callback) : nullptr),
          _lastCall(other._lastCall),
          _startTime(other._startTime),
          _repeatTime(other._repeatTime)
    {
    }

    Repeater& Repeater::operator=(const Repeater& other)
    {
        if (this != &other)
        {
            auto callback
                = other._callback ? std::make_shared<CallbackT>(*other._callback) : nullptr;
            _callback = std::move(callback);
            _lastCall = other._lastCall;
            _startTime = other._startTime;
            _repeatTime = other._repeatTime;
        }
        return *this;
    }

    void Repeater::reset()
    {
        _callback.reset();
        _startTime.reset();
        _lastCall = {};
    }

    void Repeater::startOrUpdate()
    {
        const auto now = ClockT::now();

        if (!_startTime)
        {
            _startTime = now;
            _lastCall = now;
        }

        const auto d = DurationT(now - _lastCall).count();
        if (d >= _repeatTime)
        {
            // Commit the tick before running user code. The callback may recursively update,
            // reset, replace itself, or throw; none of those cases should expose stale state.
            _lastCall = now;
            if (auto callback = _callback; callback && *callback) [[likely]]
            {
                (*callback)(d);
            }
        }
    }

    float Repeater::getTimeGap() const noexcept
    {
        if (!_startTime)
        {
            return 0;
        }

        return DurationT(_lastCall - *_startTime).count();
    }

} // namespace Core
