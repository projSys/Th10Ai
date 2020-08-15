#pragma once

namespace base
{
	class Clock
	{
	public:
		Clock();

		void update();
		time_t getTime() const;

	private:
		time_t m_time;
	};
}
