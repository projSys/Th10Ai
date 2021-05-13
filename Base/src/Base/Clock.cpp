#include "Base/Clock.h"

namespace base
{
	Time Clock::Now()
	{
		return chr::duration_cast<Time>(
			chr::steady_clock::now()
			.time_since_epoch());
	}
}
