#include "Base/Time.h"

namespace base
{
	Time Time::Now()
	{
		chrono::steady_clock::time_point tp = chrono::steady_clock::now();
		return Time(tp.time_since_epoch());
	}

	Time::Time(const chrono::steady_clock::duration& dur) :
		m_dur(dur)
	{
	}

	int64_t Time::getNanoSeconds() const
	{
		chrono::nanoseconds ns = chrono::duration_cast<chrono::nanoseconds>(m_dur);
		return ns.count();
	}

	int64_t Time::getMicroSeconds() const
	{
		chrono::microseconds ms = chrono::duration_cast<chrono::microseconds>(m_dur);
		return ms.count();
	}

	int64_t Time::getMilliSeconds() const
	{
		chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(m_dur);
		return ms.count();
	}

	int64_t Time::getSeconds() const
	{
		chrono::seconds s = chrono::duration_cast<chrono::seconds>(m_dur);
		return s.count();
	}

	int64_t Time::getMinutes() const
	{
		chrono::minutes m = chrono::duration_cast<chrono::minutes>(m_dur);
		return m.count();
	}

	int64_t Time::getHours() const
	{
		chrono::hours h = chrono::duration_cast<chrono::hours>(m_dur);
		return h.count();
	}
}
