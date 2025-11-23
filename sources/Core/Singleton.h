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

#include <memory>
#include <mutex>

#define _SINGLETONS_FRIEND(className)                                                              \
    template<class, bool>                                                                          \
    friend class ::Core::BaseSingleton;

#define SINGLETONS_FRIEND_NO_CNSTR(className) _SINGLETONS_FRIEND(className)

#define SINGLETONS_FRIEND(className)                                                               \
    _SINGLETONS_FRIEND(className)                                                                  \
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
