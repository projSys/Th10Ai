#pragma once

#include "Base/Common.h"

#include <cstdlib>
#include <algorithm>
#include <limits>
#include <type_traits>
#include <concepts>

namespace base
{
	// https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
	// https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
	template <typename T>
		requires std::floating_point<T>
	bool float_equal(T left, T right)
	{
		T diff = std::abs(left - right);
		if (diff <= std::numeric_limits<T>::epsilon())
			return true;
		T larger = std::max(std::abs(left), std::abs(right));
		if (diff <= larger * std::numeric_limits<T>::epsilon())
			return true;
		return false;
	}

	inline bool string_is_empty(const char* str)
	{
		return str == nullptr || str[0] == '\0';
	}

	inline bool string_is_empty(const wchar_t* wstr)
	{
		return wstr == nullptr || wstr[0] == L'\0';
	}

	// 等C++23
	template <typename T>
	constexpr std::underlying_type_t<T> to_underlying(T enumerator)
	{
		return static_cast<std::underlying_type_t<T>>(enumerator);
	}

	template <typename T>
	class FuncTraits
	{
	};

	template <typename C, typename R, typename... Args>
	class FuncTraits<R(__stdcall C::*)(Args...)>
	{
	public:
		using Raw_t = R(__stdcall *)(C*, Args...);
	};
}
