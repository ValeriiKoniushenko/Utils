// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#include "Delegate.h"

namespace Core
{

    DelegateSubscriber::~DelegateSubscriber()
    {
        releaseIfLastOwner();
    }

    DelegateSubscriber::DelegateSubscriber(const ID& id)
        : _id{ std::make_shared<ID>(id) }
    {
    }

    DelegateSubscriber& DelegateSubscriber::operator=(const ID& id)
    {
        if (!(getID() == id))
        {
            releaseIfLastOwner();
            _id = std::make_shared<ID>(id);
        }
        return *this;
    }

    DelegateSubscriber& DelegateSubscriber::operator=(const DelegateSubscriber& other)
    {
        if (this != &other) [[likely]]
        {
            if (!(getID() == other.getID()))
            {
                releaseIfLastOwner();
            }
            _id = other._id;
        }

        return *this;
    }

    DelegateSubscriber& DelegateSubscriber::operator=(DelegateSubscriber&& other) noexcept
    {
        if (this != &other) [[likely]]
        {
            if (!(getID() == other.getID()))
            {
                releaseIfLastOwner();
            }
            _id = std::move(other._id);
        }

        return *this;
    }

    void DelegateSubscriber::release()
    {
        if (_id && _id->isValid())
        {
            if (auto&& weak = _id->getOwner().tryLoad())
            {
                weak->unsubscribe(*_id);
            }
        }
        _id.reset();
    }

    void DelegateSubscriber::releaseIfLastOwner()
    {
        if (_id && _id.use_count() == 1)
        {
            release();
        }
        else
        {
            _id.reset();
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
        for (auto& subscriber : _pool)
        {
            subscriber.release();
        }
        _pool.clear();
    }

} // namespace Core
