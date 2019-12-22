#pragma once

#include "Base/Common.h"

#include <cstdint>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <limits>

namespace base
{
	typedef uint8_t byte_t;

	typedef float float32_t;
	typedef double float64_t;

#ifdef BASE_64BIT
	typedef int64_t int_t;
	typedef uint64_t uint_t;
	typedef float64_t float_t;
#else
	typedef int32_t int_t;
	typedef uint32_t uint_t;
	typedef float32_t float_t;
#endif

	template <typename T, typename Enable = void>
	class TypeTraits
	{
	};

	template <typename T>
	class TypeTraits<T, typename std::enable_if<std::is_integral<T>::value>::type>
	{
	public:
		typedef T value_t;

		static bool Equals(T left, T right)
		{
			return left == right;
		}
	};

	template <typename T>
	class TypeTraits<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
	{
	public:
		typedef T value_t;

		// https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
		// https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
		// 水太深
		static bool Equals(T left, T right)
		{
			// 当比较的数值很小的时候（比如小于1，接近0），绝对值比较
			T diff = std::abs(left - right);
			if (diff <= std::numeric_limits<T>::epsilon())
				return true;
			// 当比较的数值很大的时候，相对值比较
			T largest = std::max(std::abs(left), std::abs(right));
			if (diff <= largest * std::numeric_limits<T>::epsilon())
				return true;
			return false;
		}
	};
}
