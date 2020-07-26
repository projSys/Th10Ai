#pragma once

namespace base
{
	class Clock
	{
	public:
		Clock();

		time_t update();
		time_t getTime() const;

	private:
		time_t m_time;
	};
}
