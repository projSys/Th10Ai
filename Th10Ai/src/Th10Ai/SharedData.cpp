#include "Th10Ai/SharedData.h"

#include <Base/Clock.h>

namespace th
{
	namespace posix_time = boost::posix_time;

	SharedData::SharedData() :
		m_window(nullptr),
		m_hooked(false),
		m_unhooked(false),
		m_updated(false),
		m_exited(false),
		m_updateTime(0),
		m_actionUpdated(false)
	{
		memset(&m_status, 0, sizeof(StatusData));
		memset(&m_action, 0, sizeof(ActionData));
	}

	void SharedData::setWindow(HWND window)
	{
		m_window = window;
	}

	HWND SharedData::getWindow() const
	{
		return m_window;
	}

	void SharedData::activate()
	{
		if (!SetForegroundWindow(m_window))
			BASE_THROW(WindowsError());
	}

	bool SharedData::timedWaitHook(int64_t ms)
	{
		bool notTimeout = true;
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_hookMutex);
		if (!m_hooked)
		{
			posix_time::ptime absTime = interprocess::microsec_clock::universal_time()
				+ posix_time::milliseconds(ms);
			notTimeout = m_hookCond.timed_wait(lock, absTime);
		}
		return notTimeout;
	}

	void SharedData::notifyUnhook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_unhookMutex);
		m_unhooked = true;
		m_unhookCond.notify_one();
	}

	bool SharedData::waitUpdate()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_updateMutex);

		if (m_updated)
			std::cout << "¶ÁÈ¡Ì«ÂýÁË¡£" << std::endl;

		if (!m_updated && !m_exited)
			m_updateCond.wait(lock);

		m_updated = false;

		//Clock clock;
		//clock.update();
		//std::cout << clock.getTime() - m_updateTime << std::endl;

		return !m_exited;
	}

	const StatusData& SharedData::getStatus() const
	{
		return m_status;
	}

	ActionData& SharedData::getAction()
	{
		return m_action;
	}

	void SharedData::commit()
	{
		m_actionUpdated = true;
	}
}
