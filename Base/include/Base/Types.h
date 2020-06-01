#pragma once

#include "Base/Common.h"

#include <cstdint>

namespace base
{
	typedef uint8_t byte_t;

	typedef float float32_t;
	typedef double float64_t;

	static_assert(sizeof(float32_t) == 4, "Size is not correct.");
	static_assert(sizeof(float64_t) == 8, "Size is not correct.");

#ifdef BASE_64BIT
	typedef int64_t int_t;
	typedef uint64_t uint_t;
	typedef float64_t float_t;
#else
	typedef int32_t int_t;
	typedef uint32_t uint_t;
	typedef float32_t float_t;
#endif
}
