#pragma once

#include "Base/Common.h"

#include <cstdint>
#include <type_traits>

namespace base
{
	using byte_t = uint8_t;

	using float32_t = float;
	using float64_t = double;

	static_assert(sizeof(float32_t) == 4, "Size is not correct.");
	static_assert(sizeof(float64_t) == 8, "Size is not correct.");

#ifdef BASE_64BIT
	using int_t = int64_t;
	using uint_t = uint64_t;
	using float_t = float64_t;
#else
	using int_t = int32_t;
	using uint_t = uint32_t;
	using float_t = float32_t;
#endif

	template <typename T>
	constexpr std::underlying_type_t<T> enum_cast(T enumerator)
	{
		return static_cast<std::underlying_type_t<T>>(enumerator);
	}
}
