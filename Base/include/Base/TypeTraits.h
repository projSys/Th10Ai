#pragma once

#include "Base/Common.h"

#include <cstdlib>
#include <type_traits>
#include <algorithm>
#include <limits>

#include "Base/Types.h"

namespace base
{
	template <typename T, typename Enable = void>
	class TypeTraits
	{
	};

	template <typename T>
	class TypeTraits<T, std::enable_if_t<std::is_integral_v<T>>>
	{
	public:
		typedef T value_t;

		static bool Equals(T left, T right)
		{
			return left == right;
		}
	};

	template <typename T>
	class TypeTraits<T, std::enable_if_t<std::is_floating_point_v<T>>>
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
			T largest = std::max(std::abs(left), std::abs(right));
			if (diff <= largest * std::numeric_limits<T>::epsilon())
				return true;
			return false;
		}
	};
}
