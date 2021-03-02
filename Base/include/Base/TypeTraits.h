#pragma once

#include "Base/Common.h"

#include <cstdlib>
#include <type_traits>
#include <algorithm>
#include <limits>

namespace base
{
	template <typename T, typename Enable = void>
	class TypeTraits;

	template <typename T>
	class TypeTraits<T, std::enable_if_t<std::is_integral_v<T>>>
	{
	public:
		using Value_t = T;

		static bool Equal(T left, T right)
		{
			return left == right;
		}
	};

	template <typename T>
	class TypeTraits<T, std::enable_if_t<std::is_floating_point_v<T>>>
	{
	public:
		using Value_t = T;

		// https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
		// https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
		// https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
		// https://www.boost.org/doc/libs/1_73_0/libs/math/doc/html/math_toolkit/float_comparison.html
		static bool Equal(T left, T right)
		{
			T diff = std::abs(left - right);
			if (diff <= std::numeric_limits<T>::epsilon())
				return true;
			T largest = std::max(std::abs(left), std::abs(right));
			if (diff <= largest * std::numeric_limits<T>::epsilon())
				return true;
			return false;
		}
	};

	template <typename T>
	class FuncTraits;

	template <typename C, typename R, typename... Args>
	class FuncTraits<R(__stdcall C::*)(Args...)>
	{
	public:
		using Raw_t = R(__stdcall *)(C*, Args...);
	};
}
