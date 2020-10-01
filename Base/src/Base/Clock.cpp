#include "Base/Clock.h"

#include <chrono>

namespace base
{
	using namespace std::chrono;

	int64_t Clock::GetMilliseconds()
	{
		steady_clock::time_point tp = steady_clock::now();
		steady_clock::duration d = tp.time_since_epoch();
		return duration_cast<milliseconds>(d).count();
	}
}
