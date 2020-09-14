#include "Th10Hook/SharedData.h"

namespace th
{
	SharedData::SharedData()
	{
	}

	HWND SharedData::getWindow() const
	{
		return m_window;
	}

	void SharedData::notifyInit()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_initMutex);
		m_inited = true;
		m_initCond.notify_one();
	}

	void SharedData::notifyUninit()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_uninitMutex);
		m_uninited = true;
		m_uninitCond.notify_one();
	}

	void SharedData::waitUninit()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_uninitMutex);
		if (!m_uninited)
			m_uninitCond.wait(lock);
	}

	void SharedData::notifyUpdate()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_updateMutex);
		m_updated = true;
		m_updateCond.notify_one();
	}

	void SharedData::notifyExit()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_updateMutex);
		m_updated = true;
		m_exited = true;
		m_updateCond.notify_one();
	}

	StatusData& SharedData::getStatus()
	{
		return m_status;
	}

	ActionData& SharedData::getAction()
	{
		return m_action;
	}

	bool SharedData::isActionUpdated()
	{
		bool ret = m_actionUpdated;
		m_actionUpdated = false;
		return ret;
	}
}
