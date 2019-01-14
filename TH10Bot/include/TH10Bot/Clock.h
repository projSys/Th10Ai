#pragma once

#include <cpp/Singleton.h>

namespace th
{
	class Clock :
		public Singleton<Clock>
	{
	public:
		Clock();

		void update();

		time_t getTimestamp() const;
		time_t getTimespan() const;

	private:
		time_t m_timestamp;
		time_t m_timespan;
	};
}
