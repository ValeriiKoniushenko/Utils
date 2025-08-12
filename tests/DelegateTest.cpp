// MIT License
//
// Copyright (c) 2024 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Core/Delegate.h"

#include <gtest/gtest.h>

TEST(DelegateTest, SimpleTest1)
{
    Core::Delegate<void()> delegate;

    bool wasInvoked = false;

    auto id = delegate.subscribeAndGetID(
        [&]()
        {
            wasInvoked = true;
        });

    delegate.trigger();
    EXPECT_TRUE(wasInvoked);
}

TEST(DelegateTest, SimpleTest2)
{
    Core::Delegate<void()> delegate;

    bool wasInvoked = false;

    auto id = delegate.subscribeAndGetID(
        [&]()
        {
            wasInvoked = true;
        });
    EXPECT_EQ(1, delegate.getSubscriptionsCount());
    EXPECT_FALSE(delegate.isEmpty());

    delegate.trigger();
    EXPECT_TRUE(wasInvoked);

    delegate.unsubscribe(id);

    EXPECT_EQ(0, delegate.getSubscriptionsCount());
    EXPECT_TRUE(delegate.isEmpty());
}

TEST(DelegateTest, SimpleTest3)
{
    Core::Delegate<void()> delegate;
    {
        bool wasInvoked = false;

        Core::Delegate<void()>::IDGuard id = delegate.subscribeAndGetID(
            [&]()
            {
                wasInvoked = true;
            });
        delegate.trigger();
        EXPECT_TRUE(wasInvoked);
    }
    EXPECT_EQ(0, delegate.getSubscriptionsCount());
    EXPECT_TRUE(delegate.isEmpty());
}

TEST(DelegateTest, SimpleTest4)
{
    Core::Delegate<void()> delegate;
    {
        bool wasInvoked1 = false;
        bool wasInvoked2 = false;

        Core::Delegate<void()>::IDGuard id1 = delegate.subscribeAndGetID(
            [&]()
            {
                wasInvoked1 = true;
            });

        Core::Delegate<void()>::IDGuard id2 = delegate.subscribeAndGetID(
            [&]()
            {
                wasInvoked2 = true;
            });

        delegate.trigger();
        EXPECT_TRUE(wasInvoked1);
        EXPECT_TRUE(wasInvoked2);
    }
    EXPECT_EQ(0, delegate.getSubscriptionsCount());
    EXPECT_TRUE(delegate.isEmpty());
}

TEST(DelegateTest, SimpleTest5)
{
    Core::Delegate<void()> delegate;
    {
        bool wasInvoked = false;

        Core::Delegate<void()>::IDGuard id = delegate.subscribeAndGetID(
            [&]()
            {
                wasInvoked = true;
            });

        delegate.trigger();
        EXPECT_TRUE(wasInvoked);
    }
    EXPECT_EQ(0, delegate.getSubscriptionsCount());
    EXPECT_TRUE(delegate.isEmpty());

    {
        bool wasInvoked = false;

        Core::Delegate<void()>::IDGuard id = delegate.subscribeAndGetID(
            [&]()
            {
                wasInvoked = true;
            });

        delegate.trigger();
        EXPECT_TRUE(wasInvoked);
    }
    EXPECT_EQ(0, delegate.getSubscriptionsCount());
    EXPECT_TRUE(delegate.isEmpty());
}

TEST(DelegateTest, SubscribeWithoutIDGetting)
{
    Core::Delegate<void()> delegate;
    {
        bool wasInvoked = false;

        delegate.subscribe(
            [&]()
            {
                wasInvoked = true;
            });

        delegate.trigger();
        EXPECT_TRUE(wasInvoked);
    }
    EXPECT_EQ(1, delegate.getSubscriptionsCount());
    EXPECT_FALSE(delegate.isEmpty());
}
