#include "Utils/Size.h"

#include <gtest/gtest.h>

TEST(UtilsTests, Size2Area)
{
	{
		constexpr float v = 5.5f;
		constexpr Utils::Size<decltype(v), 2> a = { v, v };
		static_assert(a.area() == v * v);
	}

	{
		constexpr int v = 5;
		constexpr Utils::Size<decltype(v), 2> a = { v, v };
		static_assert(a.area() == v * v);
	}
}

TEST(UtilsTests, Size2LessGreater)
{
	{
		constexpr float v1 = 5.5f, v2 = 3.3f;
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(b < a);
		static_assert(a > b);

		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a1 = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b1 = { v1, v1 };
		static_assert(b1 <= a1);
		static_assert(a1 >= b1);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(b < a);
		static_assert(a > b);

		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a1 = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b1 = { v1, v1 };
		static_assert(b1 <= a1);
		static_assert(a1 >= b1);
	}
}

TEST(UtilsTests, Size2Equal)
{
	{
		const float v1 = 5.5f;
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v1, v1 };
		EXPECT_TRUE(a == b);
	}

	{
		constexpr int v1 = 5;
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v1, v1 };
		static_assert(a == b);
	}
}

TEST(UtilsTests, Size2NotEqual)
{
	{
		const float v1 = 5.5f, v2 = 3.3f;
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		EXPECT_TRUE(a != b);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a != b);
	}
}

TEST(UtilsTests, Size2Sum)
{
	{
		const float v1 = 5.5f, v2 = 3.3f;
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> result { v1 + v2, v1 + v2 };
		EXPECT_TRUE(a + b == result);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a + b == Utils::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 + v2, v1 + v2 });
	}
}

TEST(UtilsTests, Size2Substract)
{
	{
		const float v1 = 5.5f, v2 = 3.3f;
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> result { v1 - v2, v1 - v2 };
		EXPECT_TRUE(a - b == result);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a - b == Utils::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 - v2, v1 - v2 });
	}
}

TEST(UtilsTests, Size2Multiplication)
{
	{
		const float v1 = 5.5f, v2 = 3.3f;
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> result { v1 * v2, v1 * v2 };
		EXPECT_TRUE(a * b == result);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a * b == Utils::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 * v2, v1 * v2 });
	}
}

TEST(UtilsTests, Size2Dividing)
{
	{
		const float v1 = 5.5f, v2 = 3.3f;
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> result { v1 / v2, v1 / v2 };
		EXPECT_TRUE(a / b == result);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a / b == Utils::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 / v2, v1 / v2 });
	}
}

TEST(UtilsTests, Size2Appending)
{
	{
		const float v1 = 5.5f, v2 = 3.3f;
		Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		const Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		Utils::Size<std::remove_const_t<decltype(v1)>, 2> correct = a + b;
		EXPECT_EQ(correct, (a += b));
	}

	{
		const float v1 = 5.5f, v2 = 3.3f;
		Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		Utils::Size<std::remove_const_t<decltype(v1)>, 2> correct = { v1 + 1.f, v1 + 1.f };
		a += 1.f;
		EXPECT_EQ(correct, a);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		Utils::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		Utils::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		Utils::Size<std::remove_const_t<decltype(v1)>, 2> correct = a + b;

		EXPECT_EQ(correct, (a += b));
	}
}

TEST(UtilsTests, SizeCasting)
{
	Utils::FSize2 fsize = { 2.5f, 5.5f };
	Utils::ISize2 isize = static_cast<Utils::ISize2>(fsize);

	EXPECT_EQ(2 * 5, isize.area());
}