#pragma once

#include "Base/Common.h"

#include "Base/Types.h"

namespace base
{
	class Clock
	{
	public:
		static int64_t GetMilliseconds();
	};
}
