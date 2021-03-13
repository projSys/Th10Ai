#pragma once

#include "Th10Hook/Common.h"

#include <boost/thread.hpp>
#include <Base/Singleton.h>

namespace th
{
	class DllLoader :
		public Singleton<DllLoader>
	{
	public:
		DllLoader();

		void start();
		void join();

	private:
		void proc();

		//Logger m_logger;
		// 兼容：VS2017的std::thread会在DllMain里死锁
		boost::thread m_thread;
	};
}
