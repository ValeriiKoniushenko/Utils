#include "Core/Size.h"

#include <gtest/gtest.h>

TEST(UtilsTests, Size2Area)
{
	{
		constexpr float v = 5.5f;
		constexpr Core::Size<decltype(v), 2> a = { v, v };
		static_assert(a.area() == v * v);
	}

	{
		constexpr int v = 5;
		constexpr Core::Size<decltype(v), 2> a = { v, v };
		static_assert(a.area() == v * v);
	}
}

TEST(UtilsTests, Size2LessGreater)
{
	{
		constexpr float v1 = 5.5f, v2 = 3.3f;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(b < a);
		static_assert(a > b);

		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a1 = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b1 = { v1, v1 };
		static_assert(b1 <= a1);
		static_assert(a1 >= b1);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(b < a);
		static_assert(a > b);

		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a1 = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b1 = { v1, v1 };
		static_assert(b1 <= a1);
		static_assert(a1 >= b1);
	}
}

TEST(UtilsTests, Size2Equal)
{
	{
		constexpr float v1 = 5.5f;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v1, v1 };
		static_assert(a == b);
	}

	{
		constexpr int v1 = 5;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v1, v1 };
		static_assert(a == b);
	}
}

TEST(UtilsTests, Size2NotEqual)
{
	{
		constexpr float v1 = 5.5f, v2 = 3.3f;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a != b);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a != b);
	}
}

TEST(UtilsTests, Size2Sum)
{
	{
		constexpr float v1 = 5.5f, v2 = 3.3f;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> result { v1 + v2, v1 + v2 };
		static_assert(a + b == result);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a + b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 + v2, v1 + v2 });
	}
}

TEST(UtilsTests, Size2Substract)
{
	{
		constexpr float v1 = 5.5f, v2 = 3.3f;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> result { v1 - v2, v1 - v2 };
		static_assert(a - b == result);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a - b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 - v2, v1 - v2 });
	}
}

TEST(UtilsTests, Size2Multiplication)
{
	{
		constexpr float v1 = 5.5f, v2 = 3.3f;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> result { v1 * v2, v1 * v2 };
		static_assert(a * b == result);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a * b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 * v2, v1 * v2 });
	}
}

TEST(UtilsTests, Size2Dividing)
{
	{
		constexpr float v1 = 5.5f, v2 = 3.3f;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> result { v1 / v2, v1 / v2 };
		static_assert(a / b == result);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		constexpr Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		static_assert(a / b == Core::Size<std::remove_const_t<decltype(v1)>, 2>{ v1 / v2, v1 / v2 });
	}
}

TEST(UtilsTests, Size2Appending)
{
	{
		const float v1 = 5.5f, v2 = 3.3f;
		Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		const Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		Core::Size<std::remove_const_t<decltype(v1)>, 2> correct = a + b;
		EXPECT_EQ(correct, (a += b));
	}

	{
		const float v1 = 5.5f, v2 = 3.3f;
		Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		Core::Size<std::remove_const_t<decltype(v1)>, 2> correct = { v1 + 1.f, v1 + 1.f };
		a += 1.f;
		EXPECT_EQ(correct, a);
	}

	{
		constexpr int v1 = 5, v2 = 3;
		Core::Size<std::remove_const_t<decltype(v1)>, 2> a = { v1, v1 };
		Core::Size<std::remove_const_t<decltype(v1)>, 2> b = { v2, v2 };
		Core::Size<std::remove_const_t<decltype(v1)>, 2> correct = a + b;

		EXPECT_EQ(correct, (a += b));
	}
}

TEST(UtilsTests, SizeCasting)
{
	{
		constexpr Core::FSize2 fsize = { 2.5f, 5.5f };
		constexpr Core::ISize2 isize = static_cast<Core::ISize2>(fsize);

		static_assert(2.f * 5.f == isize.area());
	}

	{
		constexpr glm::vec2 vec(2, 5);
		constexpr glm::vec2 gvec = Core::FSize2{ 2, 5 }.toGlm();
		static_assert(vec == gvec);
	}

	{
		constexpr auto size = Core::FSize2::fromGlm(glm::vec2(1.f, 2.f));
		static_assert(std::is_same_v<Core::FSize2, std::remove_cv<decltype(size)>::type>);
		static_assert(size.width == 1.f && size.height == 2.f);
	}
}