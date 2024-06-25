
#define CORE_DEBUG
#include "Core/Rect.h"

#include <gtest/gtest.h>

TEST(UtilsTests, CreateConstexprRect)
{
	using PosT = Core::FRect::GlobalPositionT;

	constexpr Core::FRect rect1;
	constexpr Core::FRect rect2 = { 0.f, 10.f, 10.f, -10.f };
	constexpr Core::FRect rect3 = { PosT{ 0.f, 10.f } , PosT{ 10.f, -10.f } };

	rect1.requireValid();
	rect2.requireValid();
	rect3.requireValid();

	static_assert(rect1.isValid());
	static_assert(rect2.isValid());
	static_assert(rect3.isValid());
}

TEST(UtilsTests, CreateConstexprRectGetters)
{
	using PosT = Core::FRect::GlobalPositionT;

	{
		constexpr Core::FRect rect = { 0.f, 10.f, 10.f, 0.f };

		static_assert(rect.isValid());
		static_assert(PosT(0, 10) == rect.getLeftTop());
		static_assert(PosT(10, 10) == rect.getRightTop());
		static_assert(PosT(10, 0) == rect.getRightBottom());
		static_assert(PosT(0, 0) == rect.getLeftBottom());
		static_assert(PosT(5, 5) == rect.getCenter());
		static_assert(10 == rect.getWidth());
		static_assert(10 == rect.getHeight());
	}

	{
		constexpr Core::FRect rect = { 10.f, 20.f, 20.f, 10.f };

		static_assert(rect.isValid());
		static_assert(PosT(10, 20) == rect.getLeftTop());
		static_assert(PosT(20, 20) == rect.getRightTop());
		static_assert(PosT(20, 10) == rect.getRightBottom());
		static_assert(PosT(10, 10) == rect.getLeftBottom());
		static_assert(PosT(15, 15) == rect.getCenter());
	}
}

TEST(UtilsTests, CreateRect)
{
	using PosT = Core::FRect::GlobalPositionT;

	Core::FRect rect = { 0.f, 10.f, 10.f, 0.f };

	EXPECT_TRUE(rect.isValid());
	EXPECT_EQ(PosT(0, 10), rect.getLeftTop());
	EXPECT_EQ(PosT(10, 10), rect.getRightTop());
	EXPECT_EQ(PosT(10, 0), rect.getRightBottom());
	EXPECT_EQ(PosT(0, 0), rect.getLeftBottom());
	EXPECT_EQ(PosT(5, 5), rect.getCenter());
}

TEST(UtilsTests, RectPointCollision)
{
	using PosT = Core::FRect::GlobalPositionT;

	constexpr Core::FRect rect = { 0.f, 10.f, 10.f, 0.f };

	constexpr auto innerPoint = PosT{ 3.f, 3.f };
	constexpr auto outerPoint = PosT{ -3.f, -3.f };

	static_assert(rect.isContain(innerPoint));
	static_assert(!rect.isContain(outerPoint));
}

TEST(UtilsTests, RectRectCollision)
{
	constexpr Core::FRect rect1 = { 0.f, 10.f, 10.f, 0.f };
	constexpr Core::FRect rect2 = { 5.f, 15.f, 15.f, 5.f };
	constexpr Core::FRect rect3 = { -5.f, -15.f, -15.f, -5.f };

	static_assert(rect1.isContain(rect2));
	static_assert(rect2.isContain(rect1));
	static_assert(!rect2.isContain(rect3));
}