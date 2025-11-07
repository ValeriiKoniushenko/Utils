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
    INTRUSIVE_PTR_ADAPTERS(TestObject)

    int value = 0;
    int increments = 0;
    int decrements = 0;

    void onIncrementRef(CounterT ref) override { ++increments; }
    void onDecrementRef(CounterT ref) override { ++decrements; }

    void constMethod() const {}
    void nonConstMethod() {}

    ~TestObject() override { value = -999; }
    TestObject() = default;
    TestObject(int v) { value = v; }
    TestObject(int v, int i)
    {
        value = v;
        increments = i;
    }
};

TEST(IntrusivePtrTests, SimpleCreation)
{
    {
        const TestObject f;
        ASSERT_EQ(f.getHardRefCount(), 0);
    }

    {
        TestObject f;
        ASSERT_EQ(f.getHardRefCount(), 0);
    }

    {
        IntrusivePtr ptr = new TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getHardRefCount(), 1);
        ptr->constMethod();
        ptr->nonConstMethod();
    }

    {
        IntrusivePtr<const TestObject> ptr = new TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getHardRefCount(), 1);
        ptr->constMethod();
    }

    {
        IntrusivePtr<const TestObject> ptr = new const TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getHardRefCount(), 1);
        ptr->constMethod();
    }

    {
        const IntrusivePtr ptr = new const TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getHardRefCount(), 1);
        ptr->constMethod();
    }

    {
        const IntrusivePtr ptr = new TestObject;
        ASSERT_TRUE(ptr.get());
        ASSERT_EQ(ptr->getHardRefCount(), 1);
        ptr->constMethod();
    }

    {
        IntrusivePtr<const TestObject> ptr;
        ASSERT_FALSE(ptr);

        ptr = new const TestObject;
        ASSERT_TRUE(ptr);
        ASSERT_EQ(ptr->getHardRefCount(), 1);

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
    ASSERT_EQ(raw->getHardRefCount(), 0u);

    {
        IntrusivePtr<TestObject> p(raw);
        ASSERT_TRUE(p);
        ASSERT_EQ(raw->getHardRefCount(), 1u);
        ASSERT_EQ(raw->increments, 1);
    }
}

TEST(IntrusivePtrTests, CVCvalifiers)
{
    {
        IntrusivePtr<const TestObject> p = new TestObject;
        ASSERT_TRUE(p);

        // TestObject* raw = p.get();
        const TestObject* constRaw = p.get();
    }
    {
        const IntrusivePtr<TestObject> p = new TestObject;
        ASSERT_TRUE(p);

        // TestObject* raw = p.get();
        const TestObject* constRaw = p.get();
    }
    {
        IntrusivePtr<TestObject> p = new TestObject;
        ASSERT_TRUE(p);

        IntrusivePtr<const TestObject> cp1 = p.get();
        IntrusivePtr<const TestObject> cp2 = p;
        IntrusivePtr<const TestObject> cp3;
        cp3 = p;

        IntrusivePtr<const TestObject> p2 = cp2;
        // IntrusivePtr<TestObject> p3 = cp2;
        const IntrusivePtr<const TestObject> p4 = cp2;
        // const IntrusivePtr<TestObject> p5 = cp2;
    }
}

TEST(IntrusivePtrTests, CopyIncrementsReference)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p1(raw);
        ASSERT_EQ(raw->getHardRefCount(), 1u);

        {
            IntrusivePtr<TestObject> p2 = p1;
            ASSERT_EQ(raw->getHardRefCount(), 2u);
            ASSERT_EQ(raw->increments, 2);
        }
        ASSERT_EQ(raw->getHardRefCount(), 1u);
    }
}

TEST(IntrusivePtrTests, MoveTransfersOwnership)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p1(raw);
        ASSERT_EQ(raw->getHardRefCount(), 1u);

        IntrusivePtr<TestObject> p2 = std::move(p1);
        ASSERT_FALSE(p1);
        ASSERT_TRUE(p2);
        ASSERT_EQ(raw->getHardRefCount(), 1u);
    }
}

TEST(IntrusivePtrTests, ResetReplacesPointer)
{
    auto* raw1 = new TestObject();
    auto* raw2 = new TestObject();

    {
        IntrusivePtr<TestObject> p(raw1);
        ASSERT_EQ(raw1->getHardRefCount(), 1u);

        p.reset(raw2);
        ASSERT_EQ(raw2->getHardRefCount(), 1u);
    }
}

TEST(IntrusivePtrTests, ResetNullClearsPointer)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr<TestObject> p(raw);
        ASSERT_EQ(raw->getHardRefCount(), 1u);

        p.reset();
        ASSERT_FALSE(p);
    }
}

TEST(IntrusivePtrTests, OperatorsWork)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr p(raw);
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
        IntrusivePtr p1(raw1);
        IntrusivePtr p2(raw2);

        ASSERT_EQ(raw1->getHardRefCount(), 1u);
        ASSERT_EQ(raw2->getHardRefCount(), 1u);

        p1.swap(p2);

        ASSERT_EQ(p1.get(), raw2);
        ASSERT_EQ(p2.get(), raw1);
    }
}

TEST(IntrusivePtrTests, SelfAssignmentSafe)
{
    auto* raw = new TestObject();
    {
        IntrusivePtr p(raw);
        ASSERT_EQ(raw->getHardRefCount(), 1u);
        p.operator=(p); // self assign
        ASSERT_EQ(raw->getHardRefCount(), 1u);
    }
}

TEST(IntrusivePtrTests, MoveAssignmentReleasesOld)
{
    {
        IntrusivePtr p1(new TestObject);
        IntrusivePtr p2(new TestObject);
        ASSERT_EQ(p1->getHardRefCount(), 1u);
        ASSERT_EQ(p2->getHardRefCount(), 1u);

        p1 = std::move(p2);
        ASSERT_TRUE(p1);
        ASSERT_FALSE(p2);
    }
}

TEST(IntrusivePtrTests, CreateMethod)
{
    ASSERT_EQ(0, TestObject::Create()->value);
    ASSERT_EQ(111, TestObject::Create(111)->value);
    ASSERT_EQ(111, TestObject::Create(111, 222)->value);
    ASSERT_EQ(223, TestObject::Create(111, 222)->increments);
}

class AnotherObject : public IntrusiveRefCounter<AnotherObject>
{
    INTRUSIVE_PTR_ADAPTERS(AnotherObject)

public:
    AnotherObject()
    {
        int i = 1;
        ///
    }
    ~AnotherObject() override
    {
        int i = 1;
        ///
    }

    void constMethod() const {}
    void nonConstMethod() {}
};

TEST(IntrusivePtrTests, WeakTest)
{
    auto ptr = AnotherObject::Create();
    auto fund = [weak = WeakPtr(ptr)]()
    {
        if (auto obj = weak.tryLoad())
        {
            obj->constMethod();
            obj->nonConstMethod();
        }
    };

    fund();
    fund();
    ptr.reset();

    fund();
    fund();
}