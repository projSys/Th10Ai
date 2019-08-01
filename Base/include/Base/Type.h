#pragma once

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

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
	//define something for Windows (64-bit only)
	typedef int64_t int_t;
	typedef uint64_t uint_t;
	typedef float64_t float_t;
#else
	//define something for Windows (32-bit only)
	typedef int32_t int_t;
	typedef uint32_t uint_t;
	typedef float32_t float_t;
#endif
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
		// ˮ̫��
		static bool Equals(T left, T right)
		{
			// ���Ƚϵ���ֵ��С��ʱ�򣨱���С��1���ӽ�0��������ֵ�Ƚ�
			T diff = std::abs(left - right);
			if (diff <= std::numeric_limits<T>::epsilon())
				return true;
			// ���Ƚϵ���ֵ�ܴ��ʱ�����ֵ�Ƚ�
			T maxAbs = std::max(std::abs(left), std::abs(right));
			if (diff <= std::numeric_limits<T>::epsilon() * maxAbs)
				return true;
			return false;
		}
	};
}
