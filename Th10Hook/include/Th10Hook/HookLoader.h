#pragma once

#include "Th10Hook/Common.h"

#include <boost/thread.hpp>
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
		// VS2017的std::thread会在DllMain里死锁
		boost::thread m_thread;
	};
}
