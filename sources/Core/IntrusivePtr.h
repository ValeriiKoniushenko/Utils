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

#include "Utils/TypeTraits.h"
#include "libassert/assert.hpp"

#include <cstdint>
#include <utility>

namespace Core
{
    template<class T>
    concept IsPolicy = requires() { typename T::CounterT; };

    template<class T, IsPolicy Policy>
    class IntrusiveRefCounter;

    struct DefaultPolicy_IntrusiveRefCounter
    {
        using CounterT = uint32_t;

        template<class T>
        static void IncrementRef(IntrusiveRefCounter<T, DefaultPolicy_IntrusiveRefCounter>* obj) noexcept
        {
            obj->onIncrementRef(++obj->_refCount);
        }

        template<class T>
        static void DecrementRef(IntrusiveRefCounter<T, DefaultPolicy_IntrusiveRefCounter>* obj) noexcept
        {
            DEBUG_ASSERT(obj->_refCount > 0);
            if (obj->_refCount > 0) [[likely]]
            {
                obj->onDecrementRef(--obj->_refCount);
                if (obj->_refCount == 0)
                {
                    delete obj;
                }
            }
        }
    };

    template<class T, IsPolicy Policy = DefaultPolicy_IntrusiveRefCounter>
    class IntrusiveRefCounter
    {
    public:
        using ValueT = T;
        using PolicyT = Policy;
        using CounterT = typename Policy::CounterT;

    public:
        virtual ~IntrusiveRefCounter() = default;

        [[nodiscard]] CounterT getRefCount() const noexcept { return _refCount; }

    protected:
        constexpr IntrusiveRefCounter() = default;
        IntrusiveRefCounter(const IntrusiveRefCounter&) = default;
        IntrusiveRefCounter& operator=(const IntrusiveRefCounter&) = default;
        IntrusiveRefCounter(IntrusiveRefCounter&& other) noexcept { *this = std::move(other); }
        IntrusiveRefCounter& operator=(IntrusiveRefCounter&&) noexcept
        {
            static_assert(Utils::always_false<T>::value,
                          "You can't use move constructor due to logical limitations from the "
                          "ref-counter side. Use copy instead, or move an IntrusivePtr with "
                          "your object.");
            return *this;
        }

        [[maybe_unused]] virtual void onIncrementRef(CounterT ref) {}
        [[maybe_unused]] virtual void onDecrementRef(CounterT ref) {}

    private:
        mutable CounterT _refCount = 0;

        friend void Policy::IncrementRef(IntrusiveRefCounter<T>*) noexcept;
        friend void Policy::DecrementRef(IntrusiveRefCounter<T>*) noexcept;
    };

    template<class T>
    concept IsIntrusiveConstructible = requires() { std::derived_from<T, IntrusiveRefCounter<T>>; };

    template<IsIntrusiveConstructible T>
    class IntrusivePtr final
    {
    public:
        using ValueT = T;
        using PolicyT = typename ValueT::PolicyT;
        using CounterT = typename ValueT::CounterT;

    public:
        constexpr IntrusivePtr() = default;

        IntrusivePtr(T* ptr, bool addRef = true)
            : _ptr{ ptr }
        {
            if (_ptr && addRef)
            {
                PolicyT::IncrementRef(const_cast<std::remove_const_t<T>*>(_ptr));
            }
        }

        IntrusivePtr(const IntrusivePtr& other) { *this = other; }
        IntrusivePtr(IntrusivePtr&& other) noexcept { *this = std::move(other); }
        IntrusivePtr& operator=(const IntrusivePtr& other)
        {
            if (&other != this) [[likely]]
            {
                if (_ptr)
                {
                    PolicyT::DecrementRef(const_cast<std::remove_const_t<T>*>(_ptr));
                }
                _ptr = other._ptr;
                if (_ptr)
                {
                    PolicyT::IncrementRef(const_cast<std::remove_const_t<T>*>(_ptr));
                }
            }
            return *this;
        }

        IntrusivePtr& operator=(IntrusivePtr&& other) noexcept
        {
            if (&other != this) [[likely]]
            {
                if (_ptr)
                {
                    PolicyT::DecrementRef(const_cast<std::remove_const_t<T>*>(_ptr));
                }
                _ptr = other._ptr;

                other._ptr = nullptr;
            }
            return *this;
        }

        ~IntrusivePtr() noexcept
        {
            if (_ptr)
            {
                PolicyT::DecrementRef(const_cast<std::remove_const_t<T>*>(_ptr));
            }
        }

        void reset() { IntrusivePtr().swap(*this); }

        void reset(T* other) { IntrusivePtr(other).swap(*this); }

        void reset(T* other, bool addRef) { IntrusivePtr(other, addRef).swap(*this); }

        [[nodiscard]] T* get() noexcept { return _ptr; }
        [[nodiscard]] const T* get() const noexcept { return _ptr; }

        [[nodiscard]] T* detach() noexcept
        {
            T* ret = _ptr;
            _ptr = nullptr;
            return ret;
        }

        [[nodiscard]] const T& operator*() const
        {
            DEBUG_ASSERT(_ptr != nullptr);
            return *_ptr;
        }

        [[nodiscard]] const T* operator->() const
        {
            DEBUG_ASSERT(_ptr != nullptr);
            return _ptr;
        }

        [[nodiscard]] T& operator*()
        {
            DEBUG_ASSERT(_ptr != nullptr);
            return *_ptr;
        }

        [[nodiscard]] T* operator->()
        {
            DEBUG_ASSERT(_ptr != nullptr);
            return _ptr;
        }

        [[nodiscard]] operator bool() const noexcept { return _ptr != nullptr; }
        [[nodiscard]] bool isValid() const noexcept { return _ptr != nullptr; }

        void swap(IntrusivePtr& other) noexcept
        {
            T* tmp = _ptr;
            _ptr = other._ptr;
            other._ptr = tmp;
        }

    private:
        T* _ptr = nullptr;
    };

} // namespace Core