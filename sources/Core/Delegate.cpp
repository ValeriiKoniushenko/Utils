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

#include "Delegate.h"

namespace Core
{

    DelegateSubscriber::~DelegateSubscriber()
    {
        release();
    }

    DelegateSubscriber::DelegateSubscriber(const ID& id)
        : _id{ id }
    {
    }

    DelegateSubscriber& DelegateSubscriber::operator=(const ID& id)
    {
        release();
        _id = id;
        return *this;
    }

    DelegateSubscriber& DelegateSubscriber::operator=(const DelegateSubscriber& other)
    {
        if (this != &other) [[likely]]
        {
            _id = other._id;
        }

        return *this;
    }

    DelegateSubscriber& DelegateSubscriber::operator=(DelegateSubscriber&& other) noexcept
    {
        if (this != &other) [[likely]]
        {
            _id = std::move(other._id);
        }

        return *this;
    }

    void DelegateSubscriber::release()
    {
        if (_id.isValid())
        {
            if (auto&& weak = _id.getOwner().tryLoad())
            {
                weak->unsubscribe(_id);
            }
        }
    }

    void DelegateSubscriberPoolGuard::add(DelegateSubscriber&& subscriber)
    {
        _pool.emplace_back(std::move(subscriber));
    }

    void DelegateSubscriberPoolGuard::add(DelegateSubscriber& subscriber)
    {
        _pool.emplace_back(subscriber);
    }

    void DelegateSubscriberPoolGuard::clearAndReleaseAll()
    {
        _pool.clear();
    }

} // namespace Core
