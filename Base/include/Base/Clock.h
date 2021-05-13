#pragma once

#include "Base/Common.h"

#include <chrono>

namespace base
{
	namespace chr = std::chrono;

	using Time = chr::milliseconds;

	class Clock
	{
	public:
		static Time Now();
	};
}
