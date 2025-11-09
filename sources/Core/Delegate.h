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

#include "IntrusivePtr.h"

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Core
{

    class AbstractDelegate : public IntrusiveRefCounter<AbstractDelegate>
    {
    public:
        /**
         * @brief using to identify & control of the attached Delegate
         */
        class ID final
        {
        public:
            using IdT = uint32_t;
            static constexpr IdT invalidID = ~(static_cast<IdT>(0));

            struct Hasher final
            {
                uint64_t operator()(const ID& value) const { return std::hash<IdT>()(value._id); }
            };

        public:
            ID() = default;
            ID(const ID&) = default;
            ID& operator=(const ID&) = default;
            ID(ID&&) noexcept = default;
            ID& operator=(ID&&) noexcept = default;

            ID(AbstractDelegate* owner, IdT newId)
                : _owner{ owner },
                  _id{ newId }
            {
            }

            ~ID() = default;

            [[nodiscard]] constexpr bool operator==(const ID& value) const noexcept
            {
                return value._id == _id && value._owner == _owner;
            }

            [[nodiscard]] WeakPtr<AbstractDelegate> getOwner() noexcept { return _owner; }
            [[nodiscard]] bool isValid() { return _id != invalidID && _owner; }

            [[nodiscard]] IdT getId() const noexcept { return _id; }

            void _invalidate()
            {
                _id = invalidID;
                _owner.reset();
            }

        private:
            WeakPtr<AbstractDelegate> _owner;
            IdT _id = invalidID;
        }; // class ID

    public:
        virtual void unsubscribe(ID& id) = 0;
    };

    template<class F>
    class Delegate final : public AbstractDelegate
    {
        INTRUSIVE_PTR_ADAPTERS(Delegate)

    public:
        using CallbackT = std::function<F>;
        using CallbackContainerT = std::unordered_map<ID::IdT, CallbackT>;

    public:
        ~Delegate() override = default;
        Delegate(const Delegate&) = default;

        Delegate(Delegate&&) = delete;

        template<class... TArgs>
        void trigger(TArgs&&... args)
        {
            for (auto&& [id, callback] : _callbacks)
            {
                std::invoke(callback, std::forward<TArgs>(args)...);
            }
        }

        [[nodiscard]] ID subscribeAndGetID(CallbackT&& callback)
        {
            ID id(this, ++_generatedID);
            _callbacks.emplace(id.getId(), std::forward<CallbackT>(callback));
            return id;
        }

        void subscribe(CallbackT&& callback)
        {
            ID id(nullptr, ++_generatedID);
            _callbacks.emplace(id.getId(), std::forward<CallbackT>(callback));
        }

        template<class RefObjectT, class ClassFuncT>
        void subscribe(RefObjectT* object, ClassFuncT func)
        {
            ID id(nullptr, ++_generatedID);
            _callbacks.emplace(id.getId(), [object, func]<class... TArgs>(TArgs&&... args)
                               { std::invoke(func, *object, std::forward<TArgs>(args)...); });
        }

        template<class RefObjectT, class ClassFuncT>
        void subscribe(const IntrusivePtr<RefObjectT>& object, ClassFuncT func)
        {
            ID id(nullptr, ++_generatedID);
            _callbacks.emplace(
                id.getId(),
                [weak = WeakPtr<RefObjectT>(object), func]<class... TArgs>(TArgs&&... args)
                {
                    if (auto&& ptr = weak.tryLoad())
                    {
                        std::invoke(func, *ptr, std::forward<TArgs>(args)...);
                    }
                });
        }

        template<class RefObjectT, class ClassFuncT>
        [[nodiscard]] ID subscribeAndGetID(RefObjectT* object, ClassFuncT func)
        {
            ID id(nullptr, ++_generatedID);
            _callbacks.emplace(id.getId(), [object, func]<class... TArgs>(TArgs&&... args)
                               { std::invoke(func, *object, std::forward<TArgs>(args)...); });
            return id;
        }

        template<class RefObjectT, class ClassFuncT>
        [[nodiscard]] ID subscribeAndGetID(const IntrusivePtr<RefObjectT>& object, ClassFuncT func)
        {
            ID id(nullptr, ++_generatedID);
            _callbacks.emplace(
                id.getId(),
                [weak = WeakPtr<RefObjectT>(object), func]<class... TArgs>(TArgs&&... args)
                {
                    if (auto&& ptr = weak.tryLoad())
                    {
                        std::invoke(func, *ptr, std::forward<TArgs>(args)...);
                    }
                });
            return id;
        }

        void unsubscribe(ID& id) override
        {
            _callbacks.erase(id.getId());
            id._invalidate();
        }

        [[nodiscard]] typename CallbackContainerT::size_type getSubscriptionsCount() const noexcept
        {
            return _callbacks.size();
        }
        [[nodiscard]] bool isEmpty() const noexcept { return _callbacks.empty(); }

        void reset() { _callbacks.clear(); }

        [[nodiscard]] typename ID::IdT getLastGeneratedID() const noexcept { return _generatedID; }

    private:
        // Use Delegate<..>::Create() to create an object
        Delegate() = default;

    private:
        CallbackContainerT _callbacks{};
        typename ID::IdT _generatedID = ID::invalidID;
    };

    class DelegateSubscriber final
    {
    public:
        using ID = AbstractDelegate::ID;

    public:
        DelegateSubscriber() = default;
        ~DelegateSubscriber();

        DelegateSubscriber(const ID& id);
        DelegateSubscriber& operator=(const ID& id);

        DelegateSubscriber(const DelegateSubscriber& other) { *this = other; }
        DelegateSubscriber& operator=(const DelegateSubscriber& other);
        DelegateSubscriber(DelegateSubscriber&& other) noexcept { *this = std::move(other); }
        DelegateSubscriber& operator=(DelegateSubscriber&& other) noexcept;

        void release();

        [[nodiscard]] ID& getID() noexcept { return _id; }
        [[nodiscard]] const ID& getID() const noexcept { return _id; }

    private:
        ID _id;
    };

    class DelegateSubscriberPoolGuard final
    {
    public:
        DelegateSubscriberPoolGuard() = default;
        ~DelegateSubscriberPoolGuard() = default;
        DelegateSubscriberPoolGuard(const DelegateSubscriberPoolGuard&) = default;
        DelegateSubscriberPoolGuard(DelegateSubscriberPoolGuard&&) noexcept = default;

        void add(DelegateSubscriber&& subscriber);
        void add(DelegateSubscriber& subscriber);
        void operator<<(DelegateSubscriber&& subscriber) { add(std::move(subscriber)); }
        void operator<<(DelegateSubscriber& subscriber) { add(subscriber); }

        void clearAndReleaseAll();
        [[nodiscard]] bool isEmpty() const noexcept { return _pool.empty(); }
        [[nodiscard]] std::size_t size() const noexcept { return _pool.size(); }

    private:
        std::vector<DelegateSubscriber> _pool;
    };

} // namespace Core
