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

		// VS2017��std::thread����DllMain������
		boost::thread m_thread;
	};
}
