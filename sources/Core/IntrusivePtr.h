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
    //
    // ___  ___ _
    // |  \/  |(_)
    // | .  . | _  ___   ___
    // | |\/| || |/ __| / __|
    // | |  | || |\__ \| (__
    // \_|  |_/|_||___/ \___|

    template<class T>
    concept IsPolicy = requires() {
        typename T::CounterT;
        std::is_invocable_v<decltype(T::IncrementRef)>;
        std::is_invocable_v<decltype(T::DecrementRef)>;
    };

    template<class T, IsPolicy Policy>
    class IntrusiveRefCounter;

    template<class T, IsPolicy Policy>
    void _IncrementRefCounter(IntrusiveRefCounter<T, Policy>* obj) noexcept
    {
        Policy::IncrementRef(obj->_hardRefCount);
        obj->onIncrementRef(obj->_hardRefCount);
    }

    template<class T, IsPolicy Policy>
    void _DecrementRefCounter(IntrusiveRefCounter<T, Policy>* obj) noexcept
    {
        DEBUG_ASSERT(obj->_hardRefCount > 0);
        if (obj->_hardRefCount > 0) [[likely]]
        {
            Policy::DecrementRef(obj->_hardRefCount);
            obj->onDecrementRef(obj->_hardRefCount);
            if (obj->_hardRefCount == 0 && obj->_weakRefCount == 0)
            {
                delete obj;
            }
        }
    }

    template<class T, IsPolicy Policy>
    void _IncrementWeakRefCounter(IntrusiveRefCounter<T, Policy>* obj) noexcept
    {
        Policy::IncrementRef(obj->_weakRefCount);
    }

    template<class T, IsPolicy Policy>
    void _DecrementWeakRefCounter(IntrusiveRefCounter<T, Policy>* obj, T*& weakRawPtr) noexcept
    {
        DEBUG_ASSERT(obj->_weakRefCount > 0);
        if (obj->_weakRefCount > 0) [[likely]]
        {
            Policy::DecrementRef(obj->_weakRefCount);
            if (obj->_hardRefCount == 0 && obj->_weakRefCount == 0)
            {
                delete obj;
                weakRawPtr = nullptr;
            }
        }
    }

    //
    // ______         _  _
    // | ___ \       | |(_)
    // | |_/ /  ___  | | _   ___  _   _
    // |  __/  / _ \ | || | / __|| | | |
    // | |    | (_) || || || (__ | |_| |
    // \_|     \___/ |_||_| \___| \__, |
    //                             __/ |
    //                            |___/

    struct DefaultPolicy_IntrusiveRefCounter
    {
        using CounterT = uint32_t;
        static void IncrementRef(CounterT& counter) noexcept { ++counter; }
        static void DecrementRef(CounterT& counter) noexcept { --counter; }
    };

    //
    // ______  _
    // | ___ \| |
    // | |_/ /| |_  _ __
    // |  __/ | __|| '__|
    // | |    | |_ | |
    // \_|     \__||_|

    template<class T>
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
                _IncrementRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
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
                    _DecrementRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
                }
                _ptr = other._ptr;
                if (_ptr)
                {
                    _IncrementRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
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
                    _DecrementRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
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
                _DecrementRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
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

        template<class>
        friend class WeakPtr;
    };

    //
    //  _    _               _    ______  _
    // | |  | |             | |   | ___ \| |
    // | |  | |  ___   __ _ | | __| |_/ /| |_  _ __
    // | |/\| | / _ \ / _` || |/ /|  __/ | __|| '__|
    // \  /\  /|  __/| (_| ||   < | |    | |_ | |
    //  \/  \/  \___| \__,_||_|\_\\_|     \__||_|

    template<class T>
    class WeakData final
    {
    public:
        using ValueT = T;

    public:
        ~WeakData()
        {
            if (_ptr && !_noRefDecrement)
            {
                _DecrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr), const_cast<std::remove_const_t<T>*&>(_ptr));
            }
        }

        [[nodiscard]] T* get() noexcept { return _ptr; }
        [[nodiscard]] const T* get() const noexcept { return _ptr; }

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

    private:
        WeakData(T* ptr, bool noRefDecrement)
            : _ptr{ ptr },
              _noRefDecrement{ noRefDecrement }
        {
        }

    private:
        T* _ptr = nullptr;
        bool _noRefDecrement = false;

        template<class>
        friend class WeakPtr;
    };

    template<class T>
    class WeakPtr final
    {
    public:
        using ValueT = T;
        using PolicyT = typename ValueT::PolicyT;
        using CounterT = typename ValueT::CounterT;

    public:
        constexpr WeakPtr() = default;

        WeakPtr(const IntrusivePtr<T>& ptr)
            : _ptr{ ptr._ptr }
        {
            if (_ptr)
            {
                _IncrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
            }
        }

        WeakPtr(const WeakPtr& other) { *this = other; }
        WeakPtr(WeakPtr&& other) noexcept { *this = std::move(other); }
        WeakPtr& operator=(const WeakPtr& other)
        {
            if (&other != this) [[likely]]
            {
                if (_ptr)
                {
                    _DecrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr), _ptr);
                }
                _ptr = other._ptr;
                if (_ptr)
                {
                    _IncrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
                }
            }
            return *this;
        }

        WeakPtr& operator=(WeakPtr&& other) noexcept
        {
            if (&other != this) [[likely]]
            {
                if (_ptr)
                {
                    _DecrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr), _ptr);
                }
                _ptr = other._ptr;

                other._ptr = nullptr;
            }
            return *this;
        }

        ~WeakPtr() noexcept
        {
            if (_ptr)
            {
                _DecrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr), const_cast<std::remove_const_t<T>*&>(_ptr));
            }
        }

        void reset() { WeakPtr().swap(*this); }

        [[nodiscard]] operator bool() const noexcept { return _ptr != nullptr; }
        [[nodiscard]] bool isValid() const noexcept { return _ptr != nullptr; }

        [[nodiscard]] bool hasHardLink() const noexcept { return _ptr && _ptr->getHardRefCount() > 0; }

        void swap(WeakPtr& other) noexcept
        {
            T* tmp = _ptr;
            _ptr = other._ptr;
            other._ptr = tmp;
        }

        [[nodiscard]] WeakData<T> tryLoad() const
        {
            WeakData<T> out(_ptr, hasHardLink());
            if (!hasHardLink())
            {
                _ptr = nullptr;
            }
            return out;
        }

    private:
        mutable T* _ptr = nullptr;
    };

    //  _____                       _
    // /  __ \                     | |
    // | /  \/  ___   _   _  _ __  | |_   ___  _ __
    // | |     / _ \ | | | || '_ \ | __| / _ \| '__|
    // | \__/\| (_) || |_| || | | || |_ |  __/| |
    //  \____/ \___/  \__,_||_| |_| \__| \___||_|

    template<class T, IsPolicy Policy = DefaultPolicy_IntrusiveRefCounter>
    class IntrusiveRefCounter
    {
    public:
        using ValueT = T;
        using PolicyT = Policy;
        using CounterT = typename Policy::CounterT;

    public:
        virtual ~IntrusiveRefCounter() = default;

        template<class... ArgsT>
        [[nodiscard]] static IntrusivePtr<T> Create(ArgsT&&... args)
        {
            return IntrusivePtr<T>(new T(std::forward<ArgsT>(args)...));
        }
        template<class... ArgsT>
        [[nodiscard]] static IntrusivePtr<const T> CreateConst(ArgsT&&... args)
        {
            return IntrusivePtr<const T>(new const T(std::forward<ArgsT>(args)...));
        }

        [[nodiscard]] CounterT getHardRefCount() const noexcept { return _hardRefCount; }
        [[nodiscard]] CounterT getWeakRefCount() const noexcept { return _weakRefCount; }

    protected:
        constexpr IntrusiveRefCounter() = default;
        IntrusiveRefCounter(const IntrusiveRefCounter&) = default;
        IntrusiveRefCounter& operator=(const IntrusiveRefCounter&) = default;
        IntrusiveRefCounter(IntrusiveRefCounter&& other) noexcept { *this = std::move(other); }
        IntrusiveRefCounter& operator=(IntrusiveRefCounter&&) noexcept
        {
            static_assert(Utils::AlwaysFalse<T>::value,
                          "You can't use move constructor due to logical limitations from the "
                          "ref-counter side. Use copy instead, or move an IntrusivePtr with "
                          "your object.");
            return *this;
        }

        [[maybe_unused]] virtual void onIncrementRef(CounterT ref) {}
        [[maybe_unused]] virtual void onDecrementRef(CounterT ref) {}

    private:
        mutable CounterT _hardRefCount = 0;
        mutable CounterT _weakRefCount = 0;

        template<class _T, IsPolicy _P>
        friend void _IncrementRefCounter(IntrusiveRefCounter<_T, _P>*) noexcept;

        template<class _T, IsPolicy _P>
        friend void _DecrementRefCounter(IntrusiveRefCounter<_T, _P>*) noexcept;

        template<class _T, IsPolicy _P>
        friend void _IncrementWeakRefCounter(IntrusiveRefCounter<_T, _P>*) noexcept;

        template<class _T, IsPolicy _P>
        friend void _DecrementWeakRefCounter(IntrusiveRefCounter<_T, _P>*, _T*&) noexcept;
    };

} // namespace Core