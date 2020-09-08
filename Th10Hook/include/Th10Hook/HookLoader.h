#pragma once

#include "Th10Hook/Common.h"

#include <thread>
#include <Base/Singleton.h>

namespace th
{
	class HookLoader :
		public Singleton<HookLoader>
	{
	public:
		HookLoader();

		void start();
		void join();

	private:
		void proc();

		Logger m_logger;
		std::thread m_thread;
	};
}
