#pragma once

#include "Base/Common.h"

namespace base
{
	template <typename T>
	class FuncTraits;

	template <typename C, typename R, typename... Args>
	class FuncTraits<R(__stdcall C::*)(Args...)>
	{
	public:
		using Raw_t = R(__stdcall *)(C*, Args...);
	};
}
