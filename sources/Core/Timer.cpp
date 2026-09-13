// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

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
