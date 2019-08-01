#include "GraphCap/Common.h"
#include "GraphCap/D3D9Capturer.h"

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace gc
{
	namespace bpt = boost::posix_time;

	D3D9Capturer::D3D9Capturer()
	{
		m_memory = bip::managed_shared_memory(bip::open_only, "D3D9SharedMemory");
		m_data = m_memory.find<D3D9SharedData>("D3D9SharedData").first;
		if (m_data == nullptr)
			THROW_BASE_EXCEPTION(Exception() << err_str("D3D9SharedData not found."));
	}

	bool D3D9Capturer::capture(Image& image, time_t timeout)
	{
		bpt::ptime absTime = bpt::microsec_clock::universal_time() + bpt::milliseconds(timeout);
		bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentMutex);
		if (!m_data->presentCond.timed_wait(lock, absTime))
			return false;



		return true;
	}
}
