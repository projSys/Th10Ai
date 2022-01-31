#include "Base/Time.h"

#include <ostream>

namespace base
{
	Time Clock::Now()
	{
		return chr::duration_cast<Time>(chr::steady_clock::now().time_since_epoch());
	}

	std::ostream& operator <<(std::ostream& os, const Time& time)
	{
		os << time.count();
		return os;
	}
}
