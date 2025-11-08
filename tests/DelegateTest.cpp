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

TEST(DelegateTest, SubscribeWithoutIDGetting)
{
    auto delegate = Core::Delegate<void()>::Create();
    {
        bool wasInvoked = false;

        delegate->subscribe([&]() { wasInvoked = true; });

        delegate->trigger();
        EXPECT_TRUE(wasInvoked);
    }
    EXPECT_EQ(1, delegate->getSubscriptionsCount());
    EXPECT_FALSE(delegate->isEmpty());
}

TEST(DelegateTest, UsingOfDelegateSubscriber)
{
    auto delegate = Core::Delegate<void()>::Create();
    {
        bool wasInvoked = false;
        Core::DelegateSubscriber id = delegate->subscribeAndGetID([&]() { wasInvoked = true; });
        delegate->trigger();
        EXPECT_TRUE(wasInvoked);
    }
    EXPECT_EQ(0, delegate->getSubscriptionsCount());
    EXPECT_TRUE(delegate->isEmpty());
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
    auto delegate = Core::Delegate<void()>::Create(std::move(*delegate_));

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

namespace
{

    class DelegateFixture : public ::testing::Test
    {
    protected:
        std::stringstream output;
        std::streambuf* oldOutput = nullptr;

        void SetUp() override { oldOutput = std::cout.rdbuf(output.rdbuf()); }

        void TearDown() override { std::cout.rdbuf(oldOutput); }
    };

    void globalCallbackFunc(int i)
    {
        std::cout << i;
    }

    struct Temp
    {
        int i = 0;

        void set(int val)
        {
            i = val;
            std::cout << i;
        }
    };

    struct IntrTemp : public Core::IntrusiveRefCounter<IntrTemp>
    {
        INTRUSIVE_PTR_ADAPTERS(IntrTemp)

        int i = 0;

        void set(int val) { i = val; }
    };
} // namespace

TEST_F(DelegateFixture, DifferentTypeOfSubsciptions)
{
    auto delegate = Core::Delegate<void(int)>::Create();

    // static global function
    delegate->subscribe(globalCallbackFunc);
    delegate->trigger(1);

    ASSERT_EQ("1", output.str());

    // lambda
    delegate->subscribe([](int i) { std::cout << i; });
    delegate->trigger(2);
    ASSERT_EQ("122", output.str());

    // object's function
    Temp temp;
    delegate->subscribe(&temp, &Temp::set);
    delegate->trigger(3);
    ASSERT_EQ("122333", output.str());
    ASSERT_EQ(3, temp.i);
}

TEST_F(DelegateFixture, SubscriptionWithWeakObjectRef)
{
    auto delegate = Core::Delegate<void(int)>::Create();

    auto temp = IntrTemp::Create();

    ASSERT_EQ(1, temp->getHardRefCount());
    ASSERT_EQ(0, temp->getWeakRefCount());

    delegate->subscribe(temp, &IntrTemp::set);

    ASSERT_EQ(1, temp->getHardRefCount());
    ASSERT_EQ(1, temp->getWeakRefCount()); // 1 - in lambda's [weak...]

    delegate->trigger(4);
    ASSERT_EQ(4, temp->i);

    delegate.reset();

    ASSERT_EQ(1, temp->getHardRefCount());
    ASSERT_EQ(0, temp->getWeakRefCount());
}

TEST_F(DelegateFixture, SubscriptionWithWeakObjectRef2)
{
    auto delegate = Core::Delegate<void(int)>::Create();

    auto temp = IntrTemp::Create();

    ASSERT_EQ(1, temp->getHardRefCount());
    ASSERT_EQ(0, temp->getWeakRefCount());

    delegate->subscribe(temp, &IntrTemp::set);

    ASSERT_EQ(1, temp->getHardRefCount());
    ASSERT_EQ(1, temp->getWeakRefCount()); // 1 - in lambda's [weak...]

    auto* unsafeViewer = temp.get();
    temp.reset();

    ASSERT_EQ(0, unsafeViewer->getHardRefCount());
    ASSERT_EQ(1, unsafeViewer->getWeakRefCount()); // 1 - in lambda's [weak...]

    delegate->trigger(6);
    // The object MUST be deleted, so, can't even view
    // ASSERT_EQ(0, unsafeViewer->getHardRefCount());
    // ASSERT_EQ(0, unsafeViewer->getWeakRefCount());

    delegate->trigger(4);

    delegate.reset();
}
