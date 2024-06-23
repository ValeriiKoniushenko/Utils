// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
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

#pragma once

#include "Utils/Concepts.h"

#include "glm/glm.hpp"

namespace Core
{
	template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
	struct GlobalPosition : public glm::vec<L, T, Q>
	{
	};

	using GlobalPosition3F = GlobalPosition<3, float>;
	using GlobalPosition2F = GlobalPosition<2, float>;

	using GlobalPosition3D = GlobalPosition<3, double>;
	using GlobalPosition2D = GlobalPosition<2, double>;

	using GlobalPosition3I = GlobalPosition<3, int>;
	using GlobalPosition2I = GlobalPosition<2, int>;

} // namespace Core
