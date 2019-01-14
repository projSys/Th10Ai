#include "TH10Bot/Common.h"
#include "TH10Bot/Clock.h"

#include <chrono>

namespace th
{
	namespace sc = std::chrono;

	Clock::Clock() :
		Singleton(this),
		m_timestamp(0),
		m_timespan(0)
	{
		update();
	}

	void Clock::update()
	{
		sc::steady_clock::time_point now = sc::steady_clock::now();
		sc::nanoseconds nsSinceEpoch = now.time_since_epoch();
		sc::milliseconds msSinceEpoch = sc::duration_cast<sc::milliseconds>(nsSinceEpoch);
		time_t timestamp = msSinceEpoch.count();
		m_timespan = timestamp - m_timestamp;
		m_timestamp = timestamp;
	}

	time_t Clock::getTimestamp() const
	{
		return m_timestamp;
	}

	time_t Clock::getTimespan() const
	{
		return m_timespan;
	}
}
