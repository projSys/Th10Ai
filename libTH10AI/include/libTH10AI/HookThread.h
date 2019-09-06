#pragma once

#include <thread>
#include <atomic>
#include <memory>

namespace th
{
	class Th10Ai;

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
		std::shared_ptr<Th10Ai> m_ai;
	};
}
