// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "IntrusivePtr.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <type_traits>
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
        Delegate& operator=(const Delegate&) = delete;
        Delegate& operator=(Delegate&&) = delete;

        template<class... TArgs>
        void trigger(TArgs&&... args)
        {
            [[maybe_unused]] auto keepAlive = Ptr(this);

            std::vector<ID::IdT> callbackIDs;
            callbackIDs.reserve(_callbacks.size());
            for (const auto& entry : _callbacks)
            {
                callbackIDs.emplace_back(entry.first);
            }

            // Both loops resolve the issue if the callback will be released/unsubscribed during the
            // trigger by itself or by another thread.
            for (const auto id : callbackIDs)
            {
                const auto it = _callbacks.find(id);
                if (it == _callbacks.end())
                {
                    continue;
                }

                auto callback = it->second;
                if constexpr (std::is_invocable_v<CallbackT&, TArgs&...>)
                {
                    std::invoke(*callback, args...);
                }
                else
                {
                    std::invoke(*callback, std::forward<TArgs>(args)...);
                }
            }
        }

        [[nodiscard]] ID subscribeAndGetID(CallbackT&& callback)
        {
            if (!callback)
            {
                return {};
            }

            ID id(this, ++_generatedID);
            _callbacks.emplace(id.getId(), std::make_shared<CallbackT>(std::move(callback)));
            return id;
        }

        template<class RefObjectT, class ClassFuncT>
        [[nodiscard]] ID subscribeAndGetID(RefObjectT* object, ClassFuncT func)
        {
            if (!object)
            {
                return {};
            }
            if constexpr (std::is_pointer_v<ClassFuncT> || std::is_member_pointer_v<ClassFuncT>)
            {
                if (!func)
                {
                    return {};
                }
            }

            ID id(this, ++_generatedID);
            _callbacks.emplace(id.getId(),
                               std::make_shared<CallbackT>(
                                   [object, func]<class... TArgs>(TArgs&&... args)
                                   { std::invoke(func, *object, std::forward<TArgs>(args)...); }));
            return id;
        }

        template<class RefObjectT, class ClassFuncT>
        [[nodiscard]] ID subscribeAndGetID(const IntrusivePtr<RefObjectT>& object, ClassFuncT func)
        {
            if (!object)
            {
                return {};
            }
            if constexpr (std::is_pointer_v<ClassFuncT> || std::is_member_pointer_v<ClassFuncT>)
            {
                if (!func)
                {
                    return {};
                }
            }

            ID id(this, ++_generatedID);
            _callbacks.emplace(
                id.getId(),
                std::make_shared<CallbackT>(
                    [weak = WeakPtr<RefObjectT>(object), func]<class... TArgs>(TArgs&&... args)
                    {
                        if (auto&& ptr = weak.tryLoad())
                        {
                            std::invoke(func, *ptr, std::forward<TArgs>(args)...);
                        }
                    }));
            return id;
        }

        void unsubscribe(ID& id) override
        {
            if (id.getOwner().get() != this)
            {
                return;
            }

            _callbacks.erase(id.getId());
            id._invalidate();
        }

        [[nodiscard]] CallbackContainerT::size_type getSubscriptionsCount() const noexcept
        {
            return _callbacks.size();
        }
        [[nodiscard]] bool isEmpty() const noexcept { return _callbacks.empty(); }

        void reset() { _callbacks.clear(); }

        [[nodiscard]] ID::IdT getLastGeneratedID() const noexcept { return _generatedID; }

    private:
        using StoredCallbackContainerT = std::unordered_map<ID::IdT, std::shared_ptr<CallbackT>>;

    private:
        // Use Delegate<..>::Create() to create an object
        Delegate() = default;

    private:
        StoredCallbackContainerT _callbacks{};
        ID::IdT _generatedID = ID::invalidID;
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

        [[nodiscard]] ID& getID() noexcept { return _id ? *_id : _emptyID; }
        [[nodiscard]] const ID& getID() const noexcept { return _id ? *_id : _emptyID; }

    private:
        void releaseIfLastOwner();

    private:
        std::shared_ptr<ID> _id;
        ID _emptyID;
    };

    class DelegateSubscriberPoolGuard final
    {
    public:
        DelegateSubscriberPoolGuard() = default;
        ~DelegateSubscriberPoolGuard() = default;
        DelegateSubscriberPoolGuard(const DelegateSubscriberPoolGuard&) = default;
        DelegateSubscriberPoolGuard(DelegateSubscriberPoolGuard&&) noexcept = default;
        DelegateSubscriberPoolGuard& operator=(const DelegateSubscriberPoolGuard&) = default;
        DelegateSubscriberPoolGuard& operator=(DelegateSubscriberPoolGuard&&) noexcept = default;

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
