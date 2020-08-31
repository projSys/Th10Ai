#pragma once

#include "Base/Common.h"

#include <type_traits>

namespace base
{
	template <typename T>
	auto enum_value(T enumerator)
	{
		return static_cast<std::underlying_type_t<T>>(enumerator);
	}
}
