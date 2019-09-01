#include "GraphCap/Common.h"
#include "GraphCap/D3D9FrameSync.h"

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace gc
{
	namespace bpt = boost::posix_time;

	D3D9FrameSync::D3D9FrameSync()
	{
		m_memory = bip::managed_shared_memory(bip::open_only, "D3D9FSSharedMemory");
		m_data = m_memory.find<D3D9FSSharedData>("D3D9FSSharedData").first;
		if (m_data == nullptr)
			THROW_BASE_EXCEPTION(Exception() << err_str("D3D9FSSharedData not found."));
	}

	bool D3D9FrameSync::waitForEndScene(time_t timeout)
	{
		bpt::ptime absTime = bpt::microsec_clock::universal_time() + bpt::milliseconds(timeout);
		bip::scoped_lock<bip::interprocess_mutex> lock(m_data->endSceneMutex);
		return m_data->endSceneCond.timed_wait(lock, absTime);
	}

	bool D3D9FrameSync::waitForPresent(time_t timeout)
	{
		bpt::ptime absTime = bpt::microsec_clock::universal_time() + bpt::milliseconds(timeout);
		bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentMutex);
		return m_data->presentCond.timed_wait(lock, absTime);
	}

	void D3D9FrameSync::waitForPresent()
	{
		bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentMutex);
		while (!m_data->presentReady)
			m_data->presentCond.wait(lock);
		m_data->presentReady = false;
	}
}
