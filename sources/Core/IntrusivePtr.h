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

#include "BaseAssert.h"
#include "IntrusivePtr.h"
#include "Utils/TypeTraits.h"

#include <cstdint>
#include <utility>

#define INTRUSIVE_PTR_ADAPTERS(ClassName)                                                          \
public:                                                                                            \
    using Ptr = Core::IntrusivePtr<ClassName>;                                                     \
    using WPtr = Core::WeakPtr<ClassName>;                                                         \
    using CPtr = Core::IntrusivePtr<const ClassName>;                                              \
    using CWPtr = Core::WeakPtr<const ClassName>;                                                  \
                                                                                                   \
    /* cppcheck-suppress duplInheritedMember */                                                    \
    template<class... ArgsT>                                                                       \
    [[nodiscard]] static Ptr Create(ArgsT&&... args)                                               \
    {                                                                                              \
        return { new ClassName(std::forward<ArgsT>(args)...) };                                    \
    }                                                                                              \
    /* cppcheck-suppress duplInheritedMember */                                                    \
    template<class... ArgsT>                                                                       \
    [[nodiscard]] static CPtr CreateConst(ArgsT&&... args)                                         \
    {                                                                                              \
        return { new const ClassName(std::forward<ArgsT>(args)...) };                              \
    }

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
    class IntrusivePtr;

    // Helper to detect IntrusivePtr specialization
    template<class T>
    struct IsIntrusivePtrHelper : std::false_type
    {
    };

    template<class T>
    struct IsIntrusivePtrHelper<IntrusivePtr<T>> : std::true_type
    {
    };

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
        Assert(obj->_hardRefCount > 0);
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
        Assert(obj->_weakRefCount > 0);
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

        constexpr IntrusivePtr(T* ptr, bool addRef = true)
            : _ptr(ptr)
        {
            if (_ptr && addRef)
            {
                _IncrementRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
            }
        }

        template<class T2>
        constexpr IntrusivePtr(const IntrusivePtr<T2>& other, bool addRef = true)
            : IntrusivePtr(static_cast<T2*>(other.get()), addRef)
        {
        }

        IntrusivePtr(const IntrusivePtr<std::remove_const_t<T>>& other)
            requires std::is_const_v<T>
        {
            (void)*this->operator=(reinterpret_cast<const IntrusivePtr&>(other));
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

        constexpr ~IntrusivePtr() noexcept
        {
            if (_ptr)
            {
                _DecrementRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
            }
        }

        void reset(T* other) { IntrusivePtr(other).swap(*this); }
        void reset(T* other, bool addRef) { IntrusivePtr(other, addRef).swap(*this); }

        [[nodiscard]] constexpr T* get() const noexcept { return _ptr; }

        [[nodiscard]] T* detach() const noexcept
        {
            T* ret = _ptr;
            _ptr = nullptr;
            return ret;
        }

        [[nodiscard]] const T& operator*() const
        {
            Assert(_ptr != nullptr);
            return *_ptr;
        }

        [[nodiscard]] const T* operator->() const
        {
            Assert(_ptr != nullptr);
            return _ptr;
        }

        [[nodiscard]] T& operator*()
        {
            Assert(_ptr != nullptr);
            return *_ptr;
        }

        [[nodiscard]] T* operator->()
        {
            Assert(_ptr != nullptr);
            return _ptr;
        }

        void reset() { IntrusivePtr().swap(static_cast<IntrusivePtr&>(*this)); }

        void swap(IntrusivePtr& other) noexcept
        {
            T* tmp = _ptr;
            _ptr = other._ptr;
            other._ptr = tmp;
        }

        [[nodiscard]] constexpr operator bool() const noexcept { return _ptr != nullptr; }
        [[nodiscard]] constexpr bool isValid() const noexcept { return _ptr != nullptr; }

        template<class SubT>
        [[nodiscard]] constexpr bool operator==(const IntrusivePtr<SubT>& other) const noexcept
        {
            return _ptr == other._ptr;
        }

        template<class SubT>
        [[nodiscard]] constexpr bool operator==(const SubT* other) const noexcept
        {
            return _ptr == other;
        }

    private:
        T* _ptr = nullptr;

        template<class>
        friend class WeakPtr;

        template<class>
        friend class IntrusivePtr;

        friend class IntrusivePtr<const T>;
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
                _DecrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr),
                                         const_cast<std::remove_const_t<T>*&>(_ptr));
            }
        }

        [[nodiscard]] T* get() noexcept { return _ptr; }
        [[nodiscard]] const T* get() const noexcept { return _ptr; }

        [[nodiscard]] const T& operator*() const
        {
            Assert(_ptr != nullptr);
            return *_ptr;
        }

        [[nodiscard]] const T* operator->() const
        {
            Assert(_ptr != nullptr);
            return _ptr;
        }

        [[nodiscard]] T& operator*()
        {
            Assert(_ptr != nullptr);
            return *_ptr;
        }

        [[nodiscard]] T* operator->()
        {
            Assert(_ptr != nullptr);
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
        WeakPtr() = default;

        WeakPtr(T* ptr)
            : _ptr(ptr)
        {
            if (_ptr)
            {
                _IncrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr));
            }
        }

        WeakPtr(const IntrusivePtr<T>& ptr)
            : WeakPtr{ ptr._ptr }
        {
        }

        template<class T2>
        WeakPtr(const IntrusivePtr<T2>& other)
            : WeakPtr(static_cast<T2*>(other.get()))
        {
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
                _DecrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr),
                                         const_cast<std::remove_const_t<T>*&>(_ptr));
            }
        }

        [[nodiscard]] bool hasHardLink() const noexcept
        {
            return _ptr && _ptr->getHardRefCount() > 0;
        }

        [[nodiscard]] WeakData<T> tryLoad() const
        {
            if (!_ptr)
            {
                return WeakData<T>(nullptr, false);
            }

            if (!hasHardLink())
            {
                _DecrementWeakRefCounter(const_cast<std::remove_const_t<T>*>(_ptr),
                                         const_cast<std::remove_const_t<T>*&>(_ptr));
                _ptr = nullptr;
                return WeakData<T>(nullptr, false);
            }

            WeakData<T> out(_ptr, true);
            return out;
        }

        void reset() { WeakPtr().swap(static_cast<WeakPtr&>(*this)); }

        void swap(WeakPtr& other) noexcept
        {
            T* tmp = _ptr;
            _ptr = other._ptr;
            other._ptr = tmp;
        }

        [[nodiscard]] operator bool() const noexcept { return _ptr != nullptr; }
        [[nodiscard]] bool isValid() const noexcept { return _ptr != nullptr; }

        template<class SubT>
        [[nodiscard]] bool operator==(const WeakPtr<SubT>& other) const noexcept
        {
            return _ptr == other._ptr;
        }

        template<class SubT>
        [[nodiscard]] bool operator==(const SubT* other) const noexcept
        {
            return _ptr == other;
        }

    private:
        mutable T* _ptr = nullptr;

        template<class>
        friend class WeakPtr;
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
        INTRUSIVE_PTR_ADAPTERS(IntrusiveRefCounter)

    public:
        using ValueT = T;
        using PolicyT = Policy;
        using CounterT = typename Policy::CounterT;

    public:
        virtual ~IntrusiveRefCounter() = default;

        [[nodiscard]] constexpr CounterT getHardRefCount() const noexcept { return _hardRefCount; }
        [[nodiscard]] constexpr CounterT getWeakRefCount() const noexcept { return _weakRefCount; }

    protected:
        constexpr IntrusiveRefCounter() = default;

        constexpr IntrusiveRefCounter(const IntrusiveRefCounter&)
        {
            // Do nothing
        }

        constexpr IntrusiveRefCounter& operator=(const IntrusiveRefCounter&)
        {
            // Do nothing
            return *this;
        }

        /*!
         * You can't use move constructor due to logical limitations from the
         * ref-counter side. Use copy instead or move an IntrusivePtr with
         * your object.
         */
        IntrusiveRefCounter(IntrusiveRefCounter&& other) noexcept = delete;

        /*!
         * You can't use move constructor due to logical limitations from the
         * ref-counter side. Use copy instead or move an IntrusivePtr with
         * your object.
         */
        IntrusiveRefCounter& operator=(IntrusiveRefCounter&&) noexcept = delete;

        [[maybe_unused]] virtual void onIncrementRef(CounterT ref) {}

        [[maybe_unused]] virtual void onDecrementRef(CounterT ref) {}

    private:
        CounterT _hardRefCount = 0;
        CounterT _weakRefCount = 0;

        template<class _T, IsPolicy _P>
        friend void _IncrementRefCounter(IntrusiveRefCounter<_T, _P>*) noexcept;

        template<class _T, IsPolicy _P>
        friend void _DecrementRefCounter(IntrusiveRefCounter<_T, _P>*) noexcept;

        template<class _T, IsPolicy _P>
        friend void _IncrementWeakRefCounter(IntrusiveRefCounter<_T, _P>*) noexcept;

        template<class _T, IsPolicy _P>
        friend void _DecrementWeakRefCounter(IntrusiveRefCounter<_T, _P>*, _T*&) noexcept;
    };

    template<class NewT_, class T, class NewT = std::remove_reference_t<NewT_>>
    [[nodiscard]] constexpr IntrusivePtr<NewT> StaticCast(const IntrusivePtr<T>& ptr)
    {
        return { static_cast<NewT*>(ptr.get()) };
    }

    template<class NewT_, class T, class NewT = std::remove_reference_t<NewT_>>
    [[nodiscard]] IntrusivePtr<NewT> DynamicCast(const IntrusivePtr<T>& ptr)
    {
        return { dynamic_cast<NewT*>(ptr.get()) };
    }

    template<class NewT_, class T, class NewT = std::remove_reference_t<NewT_>>
    [[nodiscard]] constexpr IntrusivePtr<NewT> ReinterpretCast(const IntrusivePtr<T>& ptr)
    {
        return { reinterpret_cast<NewT*>(ptr.get()) };
    }

    template<class NewT_, class T, class NewT = std::remove_reference_t<NewT_>>
    [[nodiscard]] constexpr IntrusivePtr<NewT> ConstCast(const IntrusivePtr<T>& ptr)
    {
        return { const_cast<NewT*>(ptr.get()) };
    }

    template<class NewT_, class T, class NewT = std::remove_reference_t<NewT_>>
    [[nodiscard]] constexpr WeakPtr<NewT> StaticCast(const WeakPtr<T>& ptr)
    {
        return { static_cast<NewT*>(ptr.get()) };
    }

    template<class NewT_, class T, class NewT = std::remove_reference_t<NewT_>>
    [[nodiscard]] WeakPtr<NewT> DynamicCast(const WeakPtr<T>& ptr)
    {
        return { dynamic_cast<NewT*>(ptr.get()) };
    }

    template<class NewT_, class T, class NewT = std::remove_reference_t<NewT_>>
    [[nodiscard]] constexpr WeakPtr<NewT> ReinterpretCast(const WeakPtr<T>& ptr)
    {
        return { reinterpret_cast<NewT*>(ptr.get()) };
    }

    template<class NewT_, class T, class NewT = std::remove_reference_t<NewT_>>
    [[nodiscard]] constexpr WeakPtr<NewT> ConstCast(const WeakPtr<T>& ptr)
    {
        return { const_cast<NewT*>(ptr.get()) };
    }
} // namespace Core