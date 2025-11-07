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
#include "Utils/CopyableAndMoveableBehaviour.h"

#include <cstdint>
#include <functional>
#include <unordered_map>

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

        private:
            WeakPtr<AbstractDelegate> _owner;
            IdT _id = invalidID;
        }; // class ID

    public:
        virtual void unsubscribe(const ID& id) = 0;
    };

    template<class F>
    class Delegate final : public AbstractDelegate
    {
        INTRUSIVE_PTR_ADAPTERS(Delegate)

    public:
        using CallbackT = std::function<F>;
        using CallbackContainerT = std::unordered_map<ID, CallbackT, typename ID::Hasher>;
        using Ptr = IntrusivePtr<Delegate>;
        using CPtr = IntrusivePtr<const Delegate>;

    public:
        ~Delegate() override = default;

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
            _callbacks.emplace(id, std::forward<CallbackT>(callback));
            return id;
        }

        void subscribe(CallbackT&& callback)
        {
            ID id(nullptr, ++_generatedID);
            _callbacks.emplace(id, std::forward<CallbackT>(callback));
        }

        void unsubscribe(const ID& id) override { _callbacks.erase(id); }

        [[nodiscard]] typename CallbackContainerT::size_type getSubscriptionsCount() const noexcept
        {
            return _callbacks.size();
        }
        [[nodiscard]] bool isEmpty() const noexcept { return _callbacks.empty(); }

        void reset() { _callbacks.clear(); }

        [[nodiscard]] typename ID::IdT getLastGeneratedID() const noexcept { return _generatedID; }

    private:
        Delegate() = default;

    private:
        CallbackContainerT _callbacks{};
        typename ID::IdT _generatedID = ID::invalidID;
    };

    class DelegateSubscriber final
    {
    public:
        using ID = AbstractDelegate::ID;

        DelegateSubscriber() = default;
        ~DelegateSubscriber() { release(); }

        DelegateSubscriber(const ID& id)
            : _id{ id }
        {
        }

        DelegateSubscriber& operator=(const ID& id)
        {
            release();
            _id = id;
            return *this;
        }

        void release()
        {
            if (_id.isValid())
            {
                if (auto weak = _id.getOwner().tryLoad())
                {
                    weak->unsubscribe(_id);
                }
            }
        }

        [[nodiscard]] ID& getID() noexcept { return _id; }

    private:
        ID _id;
    };

} // namespace Core
