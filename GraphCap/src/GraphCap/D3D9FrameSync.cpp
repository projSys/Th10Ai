#include "GraphCap/Common.h"
#include "GraphCap/D3D9FrameSync.h"

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace gc
{
	namespace bpt = boost::posix_time;

	D3D9FrameSync::D3D9FrameSync(FrameListener* listener) :
		m_listener(listener),
		m_done(false)
	{
		m_memory = bip::managed_shared_memory(bip::open_only, "D3D9FSSharedMemory");
		m_data = m_memory.find<D3D9FSSharedData>("D3D9FSSharedData").first;
		if (m_data == nullptr)
			THROW_BASE_EXCEPTION(Exception() << err_str("D3D9FSSharedData not found."));

		m_thread = std::thread(std::bind(&D3D9FrameSync::syncProc, this));
		//m_thread2 = std::thread(std::bind(&D3D9FrameSync::syncProc2, this));
	}

	D3D9FrameSync::~D3D9FrameSync()
	{
		m_done = true;
		{
			bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentBeginMutex);
			m_data->presentBeginReady = true;
			m_data->presentBeginCond.notify_one();
		}
		if (m_thread.joinable())
			m_thread.join();

		m_done2 = true;
		{
			bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentEndMutex);
			m_data->presentEndReady = true;
			m_data->presentEndCond.notify_one();
		}
		if (m_thread2.joinable())
			m_thread2.join();
	}

	bool D3D9FrameSync::waitForPresent(time_t timeout)
	{
		bpt::ptime absTime = bpt::microsec_clock::universal_time() + bpt::milliseconds(timeout);
		bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentBeginMutex);
		return m_data->presentBeginCond.timed_wait(lock, absTime);
	}

	bool D3D9FrameSync::waitForPresent()
	{
		bool waited = false;
		bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentBeginMutex);
		while (!m_data->presentBeginReady)
		{
			m_data->presentBeginCond.wait(lock);
			waited = true;
		}
		m_data->presentBeginReady = false;
		return waited;
	}

	void D3D9FrameSync::syncProc()
	{
		while (true)
		{
			bool waited = false;
			{
				bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentBeginMutex);
				while (!m_data->presentBeginReady)
				{
					m_data->presentBeginCond.wait(lock);
					waited = true;
				}
				m_data->presentBeginReady = false;
			}
			//std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
			//time_t e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - m_data->m_t0).count();
			//std::cout << e1 << std::endl;
			if (!m_done)
				m_listener->onPresentBegin(waited);
			else
				break;

			//waited = false;
			//{
			//	bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentEndMutex);
			//	while (!m_data->presentEndReady)
			//	{
			//		m_data->presentEndCond.wait(lock);
			//		waited = true;
			//	}
			//	m_data->presentEndReady = false;
			//}
			//if (!m_done)
			//	m_listener->onPresentEnd(waited);
			//else
			//	break;
		}
	}

	void D3D9FrameSync::syncProc2()
	{
		while (true)
		{
			bool waited = false;
			{
				bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentEndMutex);
				while (!m_data->presentEndReady)
				{
					m_data->presentEndCond.wait(lock);
					waited = true;
				}
				m_data->presentEndReady = false;
			}
			if (!m_done)
				m_listener->onPresentEnd(waited);
			else
				break;
		}
	}
}
