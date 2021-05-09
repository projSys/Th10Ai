#include "Base/Clock.h"

namespace base
{
	Time Clock::Now()
	{
		chrono::steady_clock::time_point tp = chrono::steady_clock::now();
		return tp.time_since_epoch();
	}
}
