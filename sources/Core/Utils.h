#pragma once

#include "Utils/Concepts.h"
#include <limits>
#include <cmath>

namespace Core
{
	template<Utils::IsFloating T, T Epsilon = std::numeric_limits<T>::epsilon()>
	[[nodiscard]] inline bool IsEqual(T n1, T n2) noexcept
	{
		return std::fabs(n1 - n2) < Epsilon;
	}
} // namespace Core
