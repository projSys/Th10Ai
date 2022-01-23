#pragma once

#include "Base/Common.h"

#include <cstddef>	// nullptr_t, byte
#include <cstdint>

namespace base
{
	using byte_t = uint8_t;

	using float32_t = float;
	using float64_t = double;

	static_assert(sizeof(float32_t) == 4);
	static_assert(sizeof(float64_t) == 8);

#ifdef BASE_64BIT
	using int_t = int64_t;
	using uint_t = uint64_t;
	using float_t = float64_t;
#else
	using int_t = int32_t;
	using uint_t = uint32_t;
	using float_t = float32_t;
#endif

#define _I8(x) INT8_C(x)
#define _I16(x) INT16_C(x)
#define _I32(x) INT32_C(x)
#define _I64(x) INT64_C(x)

#define _UI8(x) UINT8_C(x)
#define _UI16(x) UINT16_C(x)
#define _UI32(x) UINT32_C(x)
#define _UI64(x) UINT64_C(x)

#define _F32(x) (x ## F)
#define _F64(x) (x)

#ifdef BASE_64BIT
#define _I(x) _I64(x)
#define _UI(x) _UI64(x)
#define _F(x) _F64(x)
#else
#define _I(x) _I32(x)
#define _UI(x) _UI32(x)
#define _F(x) _F32(x)
#endif

	using charA_t = char;
	using charW_t = wchar_t;

#define _CA(x) x
#define _CW(x) _MACRO_CONCAT(L, x)
#define _C8(x) _MACRO_CONCAT(u8, x)
#define _C16(x) _MACRO_CONCAT(u, x)
#define _C32(x) _MACRO_CONCAT(U, x)

}
