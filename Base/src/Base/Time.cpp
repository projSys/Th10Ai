#include "Base/Time.h"

#include <ostream>

namespace base
{
	Time Clock::Now()
	{
		return chr::duration_cast<Time>(
			chr::steady_clock::now().time_since_epoch());
	}

	std::ostream& operator <<(std::ostream& out, const Time& time)
	{
		out << time.count();
		return out;
	}
}
