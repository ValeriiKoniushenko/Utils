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

#include "Core/IntrusivePtr.h"

#include <gtest/gtest.h>

using namespace Core;

struct TestObject : public IntrusiveRefCounter<TestObject>
{
    int value = 0;
    int increments = 0;
    int decrements = 0;

    void onIncrementRef(CounterT ref) override { ++increments; }
    void onDecrementRef(CounterT ref) override { ++decrements; }

    void constMethod() const {}
    void nonConstMethod() {}

    ~TestObject() override { value = -999; }
};

TEST(IntrusivePtrTests, SimpleCreation)
{
    {
        const TestObject f;
        ASSERT_EQ(f.getRefCount(), 0);
    }

    {
        TestObject f;
        ASSERT_EQ(f.getRefCount(), 0);
    }

    {
        IntrusivePtr ptr = new TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getRefCount(), 1);
        ptr->constMethod();
        ptr->nonConstMethod();
    }

    {
        IntrusivePtr<const TestObject> ptr = new TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getRefCount(), 1);
        ptr->constMethod();
    }

    {
        IntrusivePtr<const TestObject> ptr = new const TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getRefCount(), 1);
        ptr->constMethod();
    }

    {
        const IntrusivePtr ptr = new const TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getRefCount(), 1);
        ptr->constMethod();
    }

    {
        const IntrusivePtr ptr = new TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getRefCount(), 1);
        ptr->constMethod();
    }

    {
        IntrusivePtr<const TestObject> ptr;
        ASSERT_FALSE(ptr);

        ptr = new const TestObject;
        ASSERT_TRUE(ptr);
        ASSERT_EQ(ptr->getRefCount(), 1);

        ptr.reset();
        ASSERT_FALSE(ptr);
    }
}

TEST(IntrusivePtrTests, DefaultConstructedIsNull)
{
    IntrusivePtr<TestObject> ptr;
    ASSERT_FALSE(ptr);
    ASSERT_EQ(ptr.get(), nullptr);
}

TEST(IntrusivePtrTests, ConstructTakesOwnershipAndIncrements)
{
    auto* raw = new TestObject();
    ASSERT_EQ(raw->getRefCount(), 0u);

    {
        IntrusivePtr<TestObject> p(raw);
        ASSERT_TRUE(p);
        ASSERT_EQ(raw->getRefCount(), 1u);
        ASSERT_EQ(raw->increments, 1);
    }
}

TEST(IntrusivePtrTests, CopyIncrementsReference)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p1(raw);
        ASSERT_EQ(raw->getRefCount(), 1u);

        {
            IntrusivePtr<TestObject> p2 = p1;
            ASSERT_EQ(raw->getRefCount(), 2u);
            ASSERT_EQ(raw->increments, 2);
        }
        ASSERT_EQ(raw->getRefCount(), 1u);
    }
}

TEST(IntrusivePtrTests, MoveTransfersOwnership)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p1(raw);
        ASSERT_EQ(raw->getRefCount(), 1u);

        IntrusivePtr<TestObject> p2 = std::move(p1);
        ASSERT_FALSE(p1);
        ASSERT_TRUE(p2);
        ASSERT_EQ(raw->getRefCount(), 1u);
    }
}

TEST(IntrusivePtrTests, ResetReplacesPointer)
{
    auto* raw1 = new TestObject();
    auto* raw2 = new TestObject();

    {
        IntrusivePtr<TestObject> p(raw1);
        ASSERT_EQ(raw1->getRefCount(), 1u);

        p.reset(raw2);
        ASSERT_EQ(raw2->getRefCount(), 1u);
    }
}

TEST(IntrusivePtrTests, ResetNullClearsPointer)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p(raw);
        ASSERT_EQ(raw->getRefCount(), 1u);

        p.reset();
        ASSERT_FALSE(p);
    }
}

TEST(IntrusivePtrTests, DetachReleasesWithoutDecrementing)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p(raw);
        ASSERT_EQ(raw->getRefCount(), 1u);

        TestObject* detached = p.detach();
        ASSERT_EQ(detached, raw);
        ASSERT_FALSE(p);

        DefaultPolicy_IntrusiveRefCounter::DecrementRef(detached);
    }
}

TEST(IntrusivePtrTests, OperatorsWork)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p(raw);
        ASSERT_EQ((*p).value, 0);
        p->value = 42;
        ASSERT_EQ(p->value, 42);
    }
}

TEST(IntrusivePtrTests, SwapExchangesPointers)
{
    auto* raw1 = new TestObject();
    auto* raw2 = new TestObject();

    {
        IntrusivePtr<TestObject> p1(raw1);
        IntrusivePtr<TestObject> p2(raw2);

        ASSERT_EQ(raw1->getRefCount(), 1u);
        ASSERT_EQ(raw2->getRefCount(), 1u);

        p1.swap(p2);

        ASSERT_EQ(p1.get(), raw2);
        ASSERT_EQ(p2.get(), raw1);
    }
}

TEST(IntrusivePtrTests, SelfAssignmentSafe)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p(raw);
        ASSERT_EQ(raw->getRefCount(), 1u);
        p = p; // self assign
        ASSERT_EQ(raw->getRefCount(), 1u);
    }
}

TEST(IntrusivePtrTests, MoveAssignmentReleasesOld)
{
    {
        IntrusivePtr<TestObject> p1(new TestObject);
        IntrusivePtr<TestObject> p2(new TestObject);
        ASSERT_EQ(p1->getRefCount(), 1u);
        ASSERT_EQ(p2->getRefCount(), 1u);

        p1 = std::move(p2);
        ASSERT_TRUE(p1);
        ASSERT_FALSE(p2);
    }
}