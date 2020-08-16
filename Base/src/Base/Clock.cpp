#include "Base/Common.h"
#include "Base/Clock.h"

#include <chrono>

namespace base
{
	namespace chrono = std::chrono;

	Clock::Clock() :
		m_time(0)
	{
	}

	void Clock::update()
	{
		chrono::steady_clock::time_point tp = chrono::steady_clock::now();
		chrono::steady_clock::duration d = tp.time_since_epoch();
		m_time = chrono::duration_cast<chrono::milliseconds>(d).count();
	}

	time_t Clock::getTime() const
	{
		return m_time;
	}
}
