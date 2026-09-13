// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include <memory>
#include <mutex>

#define _SINGLETONS_FRIEND(className)                                                              \
    template<class, bool>                                                                          \
    friend class ::Core::BaseSingleton;

#define SINGLETONS_FRIEND_NO_CNSTR(className) _SINGLETONS_FRIEND(className)

#define SINGLETONS_FRIEND(className)                                                               \
    _SINGLETONS_FRIEND(className)                                                                  \
    className(const className&) = delete;                                                          \
    className(className&&) = delete;                                                               \
    className& operator=(const className&) = delete;                                               \
    className& operator=(className&&) = delete;                                                    \
                                                                                                   \
private:                                                                                           \
    className() = default;

namespace Core
{

    template<class T, bool IsTreadSafe>
    class BaseSingleton
    {
    public:
        static T& Instance()
        {
            static std::unique_ptr<T> object;

            if (!object) [[unlikely]]
            {
                if constexpr (IsTreadSafe)
                {
                    static std::mutex mutex;
                    const std::scoped_lock lg(mutex);
                    if (!object)
                    {
                        object = std::unique_ptr<T>(new T);
                    }
                }
                else
                {
                    object = std::unique_ptr<T>(new T);
                }
            }

            return *object.get();
        }

        virtual ~BaseSingleton() = default;

    protected:
        BaseSingleton() = default;
        BaseSingleton(BaseSingleton&&) = delete;
        BaseSingleton(const BaseSingleton&) = delete;
    };

    template<class T>
    using Singleton = BaseSingleton<T, false>;

    template<class T>
    using ThreadSafeSingleton = BaseSingleton<T, true>;

} // namespace Core
