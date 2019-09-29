#pragma once

#include <thread>
#include <atomic>
#include <memory>

namespace th
{
	class Ai;

	class HookThread
	{
	public:
		HookThread();
		~HookThread();

		bool isDone() const;

	private:
		void hookProc();

		std::thread m_thread;
		std::atomic_bool m_done;
		std::shared_ptr<Ai> m_ai;
	};
}
