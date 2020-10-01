#include "Th10Ai/SharedData.h"

#include <Base/Clock.h>

namespace th
{
	namespace posix_time = boost::posix_time;

	SharedData::SharedData() :
		m_window(nullptr),
		m_inited(false),
		m_uninited(false),
		m_updated(false),
		m_exited(false),
		m_updateTime(0),
		m_actionUpdated(false)
	{
		memset(&m_status, 0, sizeof(StatusData));
		memset(&m_action, 0, sizeof(ActionData));
		m_status.inputFrame = 1;
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

	bool SharedData::waitInit(int64_t ms)
	{
		bool notTimeout = true;
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_initMutex);
		if (!m_inited)
		{
			posix_time::ptime absTime = interprocess::microsec_clock::universal_time()
				+ posix_time::milliseconds(ms);
			notTimeout = m_initCond.timed_wait(lock, absTime);
		}
		return notTimeout;
	}

	void SharedData::notifyUninit()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_uninitMutex);
		m_uninited = true;
		m_uninitCond.notify_one();
	}

	bool SharedData::waitUpdate()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_updateMutex);
		if (!m_updated)
			m_updateCond.wait(lock);
		else
			std::cout << "¶ÁÈ¡Ì«ÂýÁË¡£" << std::endl;

		m_updated = false;

		//int64_t now = Clock::GetMilliseconds();
		//std::cout << now - m_updateTime << std::endl;

		return !m_exited;
	}

	StatusData& SharedData::getStatus()
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
