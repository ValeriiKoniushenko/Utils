/*
 * MIT License
 *
 * Copyright (c) 2018-2026 Valerii Koniushenko
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

#include "Core/Timer.h"

#include <chrono>
#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace
{
    using namespace std::chrono_literals;

    struct StatefulCallback
    {
        void operator()(float) { outputs->push_back(++invocationCount); }

        std::vector<int>* outputs = nullptr;
        int invocationCount = 0;
    };

    static_assert(std::is_same_v<Core::FStopwatch, Core::Stopwatch<float>>);
    static_assert(std::is_same_v<Core::DStopwatch, Core::Stopwatch<double>>);

    TEST(StopwatchTest, ConstructionStartsAUsableStopwatch)
    {
        const Core::DStopwatch stopwatch;

        const auto elapsed = stopwatch.stop();

        EXPECT_GE(elapsed, 0.0);
        EXPECT_LT(elapsed, 1.0);
    }

    TEST(StopwatchTest, MeasuresElapsedTimeUsingSeconds)
    {
        // tag::stopwatch[]
        Core::DStopwatch stopwatch;
        stopwatch.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        const auto elapsed = stopwatch.stop();

        EXPECT_GE(elapsed, 0.005);
        EXPECT_LT(elapsed, 1.0);
        // end::stopwatch[]
    }

    TEST(StopwatchTest, StartRestartsTheMeasurement)
    {
        Core::DStopwatch stopwatch;
        std::this_thread::sleep_for(5ms);
        ASSERT_GE(stopwatch.stop(), 0.005);

        stopwatch.start();
        const auto elapsedAfterRestart = stopwatch.stop();

        EXPECT_GE(elapsedAfterRestart, 0.0);
        EXPECT_LT(elapsedAfterRestart, 1.0);
    }

    TEST(StopwatchTest, StopDoesNotPauseTheMeasurement)
    {
        const Core::DStopwatch stopwatch;
        const auto firstReading = stopwatch.stop();
        std::this_thread::sleep_for(2ms);

        EXPECT_GT(stopwatch.stop(), firstReading);
    }

    TEST(RepeaterTest, StoresAndUpdatesTheRepeatInterval)
    {
        Core::Repeater repeater(1.25f);
        EXPECT_FLOAT_EQ(repeater.getRepeatTime(), 1.25f);

        repeater.setRepeatTime(2.5f);
        EXPECT_FLOAT_EQ(repeater.getRepeatTime(), 2.5f);
    }

    TEST(RepeaterTest, RejectsNonFiniteIntervalsWithoutChangingTheCurrentInterval)
    {
        Core::Repeater repeater(1.25f);

        EXPECT_THROW(repeater.setRepeatTime(std::numeric_limits<float>::quiet_NaN()),
                     std::invalid_argument);
        EXPECT_THROW(repeater.setRepeatTime(std::numeric_limits<float>::infinity()),
                     std::invalid_argument);
        EXPECT_THROW(Core::Repeater(-std::numeric_limits<float>::infinity()),
                     std::invalid_argument);
        EXPECT_FLOAT_EQ(repeater.getRepeatTime(), 1.25f);
    }

    TEST(RepeaterTest, UnstartedRepeaterHasNoTimeGap)
    {
        const Core::Repeater repeater(10.0f);

        EXPECT_FLOAT_EQ(repeater.getTimeGap(), 0.0f);
    }

    TEST(RepeaterTest, FirstUpdateStartsPositiveIntervalWithoutInvokingCallback)
    {
        Core::Repeater repeater(10.0f);
        int callbackCount = 0;
        repeater.setCallback([&](float) { ++callbackCount; });

        repeater.startOrUpdate();

        EXPECT_EQ(callbackCount, 0);
        EXPECT_FLOAT_EQ(repeater.getTimeGap(), 0.0f);
    }

    TEST(RepeaterTest, InvokesCallbackAfterIntervalWithASecondsDelta)
    {
        Core::Repeater repeater(0.005f);
        int callbackCount = 0;
        float callbackDelta = 0.0f;
        repeater.setCallback(
            [&](float delta)
            {
                ++callbackCount;
                callbackDelta = delta;
            });

        repeater.startOrUpdate();
        std::this_thread::sleep_for(5ms);
        repeater.startOrUpdate();

        ASSERT_EQ(callbackCount, 1);
        EXPECT_GE(callbackDelta, 0.005f);
        EXPECT_LT(callbackDelta, 1.0f);
        EXPECT_FLOAT_EQ(repeater.getTimeGap(), callbackDelta);
    }

    TEST(RepeaterTest, ZeroIntervalInvokesCallbackOnEveryUpdate)
    {
        Core::Repeater repeater;
        int callbackCount = 0;
        float firstDelta = -1.0f;
        repeater.setCallback(
            [&](float delta)
            {
                if (callbackCount == 0)
                {
                    firstDelta = delta;
                }
                ++callbackCount;
                EXPECT_TRUE(std::isfinite(delta));
                EXPECT_GE(delta, 0.0f);
                EXPECT_LT(delta, 1.0f);
            });

        repeater.startOrUpdate();
        repeater.startOrUpdate();
        repeater.startOrUpdate();

        EXPECT_EQ(callbackCount, 3);
        EXPECT_FLOAT_EQ(firstDelta, 0.0f);
    }

    TEST(RepeaterTest, DoesNotInvokeAgainBeforeTheNewInterval)
    {
        Core::Repeater repeater;
        int callbackCount = 0;
        repeater.setCallback([&](float) { ++callbackCount; });
        repeater.startOrUpdate();
        ASSERT_EQ(callbackCount, 1);

        repeater.setRepeatTime(10.0f);
        repeater.startOrUpdate();

        EXPECT_EQ(callbackCount, 1);
    }

    TEST(RepeaterTest, ResetClearsCallbackAndTimingButPreservesInterval)
    {
        Core::Repeater repeater(2.5f);
        int callbackCount = 0;
        repeater.setCallback([&](float) { ++callbackCount; });
        repeater.startOrUpdate();

        repeater.reset();
        EXPECT_FLOAT_EQ(repeater.getRepeatTime(), 2.5f);
        repeater.setRepeatTime(0.0f);
        repeater.startOrUpdate();

        EXPECT_EQ(callbackCount, 0);
        EXPECT_FLOAT_EQ(repeater.getTimeGap(), 0.0f);
    }

    TEST(RepeaterTest, CallbackCanResetItsOwnRepeater)
    {
        Core::Repeater repeater;
        int callbackCount = 0;
        repeater.setCallback(
            [&](float)
            {
                ++callbackCount;
                repeater.reset();
            });

        repeater.startOrUpdate();
        repeater.startOrUpdate();

        EXPECT_EQ(callbackCount, 1);
        EXPECT_FLOAT_EQ(repeater.getTimeGap(), 0.0f);
    }

    TEST(RepeaterTest, CallbackCanReplaceItself)
    {
        Core::Repeater repeater;
        int originalCallbackCount = 0;
        int replacementCallbackCount = 0;
        repeater.setCallback(
            [&](float)
            {
                ++originalCallbackCount;
                repeater.setCallback([&](float) { ++replacementCallbackCount; });
            });

        repeater.startOrUpdate();
        repeater.startOrUpdate();

        EXPECT_EQ(originalCallbackCount, 1);
        EXPECT_EQ(replacementCallbackCount, 1);
    }

    TEST(RepeaterTest, RecursiveUpdateDoesNotObserveAStaleLastCall)
    {
        Core::Repeater repeater;
        int callbackCount = 0;
        repeater.setCallback(
            [&](float)
            {
                ++callbackCount;
                if (callbackCount == 1)
                {
                    repeater.setRepeatTime(10.0f);
                    repeater.startOrUpdate();
                }
            });

        repeater.startOrUpdate();

        EXPECT_EQ(callbackCount, 1);
    }

    TEST(RepeaterTest, ThrowingCallbackDoesNotLeaveAStaleLastCall)
    {
        Core::Repeater repeater;
        int callbackCount = 0;
        repeater.setCallback(
            [&](float)
            {
                ++callbackCount;
                repeater.setRepeatTime(10.0f);
                throw std::runtime_error("expected test exception");
            });

        EXPECT_THROW(repeater.startOrUpdate(), std::runtime_error);
        EXPECT_NO_THROW(repeater.startOrUpdate());
        EXPECT_EQ(callbackCount, 1);
    }

    TEST(RepeaterTest, MutableCallbackStatePersistsAcrossInvocations)
    {
        Core::Repeater repeater;
        std::vector<int> outputs;
        repeater.setCallback(StatefulCallback{ .outputs = &outputs });

        repeater.startOrUpdate();
        repeater.startOrUpdate();

        EXPECT_EQ(outputs, (std::vector{ 1, 2 }));
    }

    TEST(RepeaterTest, CopiesHaveIndependentCallbackState)
    {
        Core::Repeater original;
        std::vector<int> outputs;
        original.setCallback(StatefulCallback{ .outputs = &outputs });
        original.startOrUpdate();
        Core::Repeater copy = original;

        original.startOrUpdate();
        copy.startOrUpdate();

        EXPECT_EQ(outputs, (std::vector{ 1, 2, 2 }));
    }

    TEST(RepeaterTest, CopyAssignmentPreservesIndependentCallbackStateAndHandlesSelfAssignment)
    {
        Core::Repeater original;
        Core::Repeater copy;
        std::vector<int> outputs;
        original.setCallback(StatefulCallback{ .outputs = &outputs });
        original.startOrUpdate();

        copy = original;
        original.startOrUpdate();
        copy.startOrUpdate();
        copy.operator=(copy);
        copy.startOrUpdate();

        EXPECT_EQ(outputs, (std::vector{ 1, 2, 2, 3 }));
    }

    TEST(RepeaterTest, MoveOperationsTransferTheCallback)
    {
        Core::Repeater original;
        int callbackCount = 0;
        original.setCallback([&](float) { ++callbackCount; });

        Core::Repeater moved = std::move(original);
        moved.startOrUpdate();
        Core::Repeater assigned;
        assigned = std::move(moved);
        assigned.startOrUpdate();

        EXPECT_EQ(callbackCount, 2);
    }

    TEST(RepeaterTest, EmptyCallbackIsSafe)
    {
        Core::Repeater repeater;
        repeater.setCallback({});

        EXPECT_NO_THROW(repeater.startOrUpdate());
        EXPECT_FLOAT_EQ(repeater.getTimeGap(), 0.0f);
    }
} // namespace
