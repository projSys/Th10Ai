#pragma once

#include <thread>
#include <atomic>
#include <chrono>

namespace th
{
	class Th10Ai
	{
	public:
		Th10Ai();
		~Th10Ai();

	private:
		void proc();

		std::thread m_thread;
		std::atomic_bool m_done;
	};
}
