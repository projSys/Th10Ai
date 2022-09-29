#pragma once

#include "Base/Common.h"

#include <chrono>
#include <iosfwd>

namespace base
{
	namespace chr = std::chrono;

	using Time = chr::milliseconds;

	class Clock
	{
	public:
		static Time Now();
	};

	inline std::ostream& operator <<(std::ostream& os, const Time& time)
	{
		os << time.count();
		return os;
	}
}
