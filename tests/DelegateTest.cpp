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

#include "Core/Delegate.h"

#include "Core/IntrusivePtr.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(DelegateTest, SimpleTest1)
{
    auto delegate = Core::Delegate<void()>::Create();

    bool wasInvoked = false;

    auto id = delegate->subscribeAndGetID([&]() { wasInvoked = true; });

    delegate->trigger();
    EXPECT_TRUE(wasInvoked);
}

TEST(DelegateTest, SimpleTest2)
{
    auto delegate = Core::Delegate<void()>::Create();

    bool wasInvoked = false;

    auto id = delegate->subscribeAndGetID([&]() { wasInvoked = true; });
    EXPECT_EQ(1, delegate->getSubscriptionsCount());
    EXPECT_FALSE(delegate->isEmpty());

    delegate->trigger();
    EXPECT_TRUE(wasInvoked);

    delegate->unsubscribe(id);

    EXPECT_EQ(0, delegate->getSubscriptionsCount());
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, UsingOfDelegateSubscriber)
{
    // tag::delegate_subscription[]
    auto delegate = Core::Delegate<void()>::Create();
    {
        bool wasInvoked = false;
        Core::DelegateSubscriber id = delegate->subscribeAndGetID([&]() { wasInvoked = true; });
        delegate->trigger();
        EXPECT_TRUE(wasInvoked);
    }
    EXPECT_EQ(0, delegate->getSubscriptionsCount());
    EXPECT_TRUE(delegate->isEmpty());
    // end::delegate_subscription[]
}

TEST(DelegateTest, OutOfScopeDelegate)
{
    auto delegate = Core::Delegate<void()>::Create();
    {
        Core::DelegateSubscriber id1 = delegate->subscribeAndGetID([&]() {});
        Core::DelegateSubscriber id2 = delegate->subscribeAndGetID([&]() {});
        delegate.reset();
        // Shouldn't be any crashes here!!!
    }
}

TEST(DelegateTest, OutOfScopeDelegateAndCopying)
{
    auto delegate_ = Core::Delegate<void()>::Create();
    auto delegate = Core::Delegate<void()>::Create(*delegate_);

    {
        Core::DelegateSubscriber id1 = delegate->subscribeAndGetID(
            [&]()
            {
                int i = 123;
                int is = 123;
            });
        Core::DelegateSubscriber id2 = id1;

        delegate_->trigger();
        delegate->trigger();

        delegate.reset();
        // Shouldn't be any crashes here!!!
    }
}

TEST(DelegateTest, MoveDelegateSubscriber)
{
    auto delegate = Core::Delegate<void()>::Create();

    {
        bool val = false;
        Core::DelegateSubscriber id1 = delegate->subscribeAndGetID([&]() { val = true; });

        ASSERT_FALSE(val);
        delegate->trigger();
        ASSERT_TRUE(val);

        Core::DelegateSubscriber id2 = std::move(id1);
        val = false;

        ASSERT_FALSE(val);
        delegate->trigger();
        ASSERT_TRUE(val);
    }
}

TEST(DelegateTest, DelegateSubscriberPool)
{
    auto delegate = Core::Delegate<void()>::Create();

    {
        ASSERT_EQ(0, delegate->getSubscriptionsCount());

        Core::DelegateSubscriberPoolGuard pool;
        pool << delegate->subscribeAndGetID([&]() {});
        pool << delegate->subscribeAndGetID([&]() {});
        pool << delegate->subscribeAndGetID([&]() {});

        ASSERT_EQ(3, delegate->getSubscriptionsCount());
        pool.clearAndReleaseAll();

        ASSERT_EQ(0, delegate->getSubscriptionsCount());

        pool << delegate->subscribeAndGetID([&]() {});
        ASSERT_EQ(1, delegate->getSubscriptionsCount());
    }
    ASSERT_EQ(0, delegate->getSubscriptionsCount());
}

TEST(DelegateTest, MemberFunctionSubscriptionIsReleasedBySubscriber)
{
    struct Receiver
    {
        void receive() { ++invocationCount; }

        int invocationCount = 0;
    };

    auto delegate = Core::Delegate<void()>::Create();
    Receiver receiver;

    {
        const Core::DelegateSubscriber subscription
            = delegate->subscribeAndGetID(&receiver, &Receiver::receive);
        delegate->trigger();
        EXPECT_EQ(1, receiver.invocationCount);
    }

    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, IntrusiveMemberFunctionSubscriptionIsReleasedBySubscriber)
{
    struct Receiver : Core::IntrusiveRefCounter<Receiver>
    {
        void receive() { ++invocationCount; }

        int invocationCount = 0;
    };

    auto delegate = Core::Delegate<void()>::Create();
    Core::IntrusivePtr<Receiver> receiver(new Receiver);

    {
        const Core::DelegateSubscriber subscription
            = delegate->subscribeAndGetID(receiver, &Receiver::receive);
        delegate->trigger();
        EXPECT_EQ(1, receiver->invocationCount);
    }

    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, CopyAssignmentReleasesPreviouslyGuardedSubscription)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriber first = delegate->subscribeAndGetID([]() {});
    const Core::DelegateSubscriber second = delegate->subscribeAndGetID([]() {});
    ASSERT_EQ(2, delegate->getSubscriptionsCount());

    first = second;

    EXPECT_EQ(1, delegate->getSubscriptionsCount());
}

TEST(DelegateTest, MoveAssignmentReleasesPreviouslyGuardedSubscription)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriber first = delegate->subscribeAndGetID([]() {});
    Core::DelegateSubscriber second = delegate->subscribeAndGetID([]() {});
    ASSERT_EQ(2, delegate->getSubscriptionsCount());

    first = std::move(second);

    EXPECT_EQ(1, delegate->getSubscriptionsCount());
}

TEST(DelegateTest, UnsubscribeRejectsAnIDOwnedByAnotherDelegate)
{
    auto firstDelegate = Core::Delegate<void()>::Create();
    auto secondDelegate = Core::Delegate<void()>::Create();
    auto firstID = firstDelegate->subscribeAndGetID([]() {});
    [[maybe_unused]] auto secondID = secondDelegate->subscribeAndGetID([]() {});

    secondDelegate->unsubscribe(firstID);

    EXPECT_TRUE(firstID.isValid());
    EXPECT_EQ(1, firstDelegate->getSubscriptionsCount());
    EXPECT_EQ(1, secondDelegate->getSubscriptionsCount());
}

TEST(DelegateTest, CallbackCanUnsubscribeItselfDuringTrigger)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriber subscription;
    int invocationCount = 0;
    subscription = delegate->subscribeAndGetID(
        [&]()
        {
            ++invocationCount;
            subscription.release();
        });

    delegate->trigger();
    delegate->trigger();

    EXPECT_EQ(1, invocationCount);
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, DelegateStaysAliveUntilTriggerReturns)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::WeakPtr<Core::Delegate<void()>> weakDelegate(delegate);
    [[maybe_unused]] auto id = delegate->subscribeAndGetID(
        [&]()
        {
            delegate.reset();
            EXPECT_TRUE(weakDelegate);
        });

    delegate->trigger();

    EXPECT_FALSE(weakDelegate);
}

TEST(DelegateTest, EveryByValueCallbackReceivesTheOriginalRvalue)
{
    auto delegate = Core::Delegate<void(std::string)>::Create();
    std::vector<std::string> receivedValues;
    [[maybe_unused]] auto firstID = delegate->subscribeAndGetID(
        [&](const std::string& value) { receivedValues.push_back(value); });
    [[maybe_unused]] auto secondID = delegate->subscribeAndGetID(
        [&](const std::string& value) { receivedValues.push_back(value); });

    delegate->trigger(std::string("payload"));

    ASSERT_EQ(2, receivedValues.size());
    EXPECT_EQ("payload", receivedValues[0]);
    EXPECT_EQ("payload", receivedValues[1]);
}

TEST(DelegateTest, CallbackCanResetDelegateDuringTrigger)
{
    auto delegate = Core::Delegate<void()>::Create();
    int invocationCount = 0;
    [[maybe_unused]] auto id = delegate->subscribeAndGetID(
        [&]()
        {
            ++invocationCount;
            delegate->reset();
        });

    delegate->trigger();
    delegate->trigger();

    EXPECT_EQ(1, invocationCount);
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, CallbackAddedDuringTriggerRunsOnNextTrigger)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::AbstractDelegate::ID addedID;
    int originalInvocationCount = 0;
    int addedInvocationCount = 0;
    [[maybe_unused]] auto originalID = delegate->subscribeAndGetID(
        [&]()
        {
            ++originalInvocationCount;
            if (!addedID.isValid())
            {
                addedID = delegate->subscribeAndGetID([&]() { ++addedInvocationCount; });
            }
        });

    delegate->trigger();
    EXPECT_EQ(1, originalInvocationCount);
    EXPECT_EQ(0, addedInvocationCount);

    delegate->trigger();
    EXPECT_EQ(2, originalInvocationCount);
    EXPECT_EQ(1, addedInvocationCount);
}

TEST(DelegateTest, NullMemberFunctionReceiverIsRejected)
{
    struct Receiver
    {
        void receive() {}
    };

    auto delegate = Core::Delegate<void()>::Create();
    Receiver* receiver = nullptr;

    auto id = delegate->subscribeAndGetID(receiver, &Receiver::receive);

    EXPECT_FALSE(id.isValid());
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, NullIntrusiveMemberFunctionReceiverIsRejected)
{
    struct Receiver : Core::IntrusiveRefCounter<Receiver>
    {
        void receive() {}
    };

    auto delegate = Core::Delegate<void()>::Create();
    const Core::IntrusivePtr<Receiver> receiver;

    auto id = delegate->subscribeAndGetID(receiver, &Receiver::receive);

    EXPECT_FALSE(id.isValid());
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, EmptyCallbackIsRejected)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::Delegate<void()>::CallbackT callback;

    auto id = delegate->subscribeAndGetID(std::move(callback));

    EXPECT_FALSE(id.isValid());
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, NullMemberFunctionIsRejected)
{
    struct Receiver
    {
        void receive() {}
    };

    auto delegate = Core::Delegate<void()>::Create();
    Receiver receiver;
    void (Receiver::* const function)() = nullptr;

    auto id = delegate->subscribeAndGetID(&receiver, function);

    EXPECT_FALSE(id.isValid());
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, AssigningTheSameIDToSubscriberDoesNotUnsubscribe)
{
    auto delegate = Core::Delegate<void()>::Create();
    auto id = delegate->subscribeAndGetID([]() {});
    Core::DelegateSubscriber subscription(id);

    subscription = id;

    EXPECT_EQ(1, delegate->getSubscriptionsCount());
    EXPECT_TRUE(subscription.getID().isValid());
}

TEST(DelegateTest, MutableCallbackStatePersistsAcrossTriggers)
{
    auto delegate = Core::Delegate<void()>::Create();
    std::vector<int> observedValues;
    [[maybe_unused]] auto id = delegate->subscribeAndGetID([state = 0, &observedValues]() mutable
                                                           { observedValues.push_back(++state); });

    delegate->trigger();
    delegate->trigger();

    EXPECT_EQ((std::vector<int>{ 1, 2 }), observedValues);
}

TEST(DelegateTest, SubscriptionLivesUntilLastCopiedSubscriberIsReleased)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriber original = delegate->subscribeAndGetID([]() {});
    {
        const Core::DelegateSubscriber copy = original;
        EXPECT_EQ(1, delegate->getSubscriptionsCount());
    }

    EXPECT_EQ(1, delegate->getSubscriptionsCount());
    original.release();
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, ExplicitReleaseInvalidatesEverySubscriberCopy)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriber original = delegate->subscribeAndGetID([]() {});
    Core::DelegateSubscriber copy = original;

    original.release();

    EXPECT_TRUE(delegate->isEmpty());
    EXPECT_FALSE(copy.getID().isValid());
}

TEST(DelegateTest, SubscriberPoolKeepsCopiedSubscriptionAlive)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriberPoolGuard pool;
    {
        Core::DelegateSubscriber subscription = delegate->subscribeAndGetID([]() {});
        pool.add(subscription);
    }

    EXPECT_EQ(1, delegate->getSubscriptionsCount());
    pool.clearAndReleaseAll();
    EXPECT_TRUE(delegate->isEmpty());
}

TEST(DelegateTest, ClearAndReleaseAllCancelsExternalSubscriberCopies)
{
    auto delegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriber subscription = delegate->subscribeAndGetID([]() {});
    Core::DelegateSubscriberPoolGuard pool;
    pool.add(subscription);

    pool.clearAndReleaseAll();

    EXPECT_TRUE(delegate->isEmpty());
    EXPECT_FALSE(subscription.getID().isValid());
}

TEST(DelegateTest, CopiedDelegatesShareMutableCallbackState)
{
    auto original = Core::Delegate<void()>::Create();
    std::vector<int> observedValues;
    [[maybe_unused]] auto id = original->subscribeAndGetID([state = 0, &observedValues]() mutable
                                                           { observedValues.push_back(++state); });

    original->trigger();
    auto copy = Core::Delegate<void()>::Create(*original);
    original->trigger();
    copy->trigger();

    EXPECT_EQ((std::vector<int>{ 1, 2, 3 }), observedValues);
}

TEST(DelegateTest, RvalueReferenceCallbackReceivesMoveOnlyArgument)
{
    auto delegate = Core::Delegate<void(std::unique_ptr<int>&&)>::Create();
    std::unique_ptr<int> received;
    [[maybe_unused]] auto id = delegate->subscribeAndGetID([&](std::unique_ptr<int>&& value)
                                                           { received = std::move(value); });

    delegate->trigger(std::make_unique<int>(42));

    ASSERT_TRUE(received);
    EXPECT_EQ(42, *received);
}

TEST(DelegateTest, SubscriberPoolsSupportCopyAssignment)
{
    auto sourceDelegate = Core::Delegate<void()>::Create();
    auto destinationDelegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriberPoolGuard source;
    source << sourceDelegate->subscribeAndGetID([]() {});
    Core::DelegateSubscriberPoolGuard destination;
    destination << destinationDelegate->subscribeAndGetID([]() {});

    destination = source;

    EXPECT_EQ(1, sourceDelegate->getSubscriptionsCount());
    EXPECT_TRUE(destinationDelegate->isEmpty());
}

TEST(DelegateTest, SubscriberPoolsSupportMoveAssignment)
{
    auto sourceDelegate = Core::Delegate<void()>::Create();
    auto destinationDelegate = Core::Delegate<void()>::Create();
    Core::DelegateSubscriberPoolGuard source;
    source << sourceDelegate->subscribeAndGetID([]() {});
    Core::DelegateSubscriberPoolGuard destination;
    destination << destinationDelegate->subscribeAndGetID([]() {});

    destination = std::move(source);

    EXPECT_TRUE(source.isEmpty()); // NOLINT(bugprone-use-after-move)
    EXPECT_EQ(1, destination.size());
    EXPECT_EQ(1, sourceDelegate->getSubscriptionsCount());
    EXPECT_TRUE(destinationDelegate->isEmpty());
}
