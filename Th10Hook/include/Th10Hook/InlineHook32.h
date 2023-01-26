#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	class InlineHook32
	{
	public:
		void* attach(void* original, void* detour);
		void detach(PVOID* pointer, PVOID detour);
	};
}
