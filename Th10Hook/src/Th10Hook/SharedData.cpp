#include "Th10Hook/SharedData.h"

#include <Base/Clock.h>

#include "Th10Hook/Reader.h"

namespace th
{
	SharedData::SharedData()
	{
	}

	HWND SharedData::getWindow() const
	{
		return m_window;
	}

	void SharedData::notifyHook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_hookMutex);
		m_hooked = true;
		m_hookCond.notify_one();
	}

	void SharedData::notifyUnhook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_unhookMutex);
		m_unhooked = true;
		m_unhookCond.notify_one();
	}

	void SharedData::waitUnhook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_unhookMutex);
		if (!m_unhooked)
			m_unhookCond.wait(lock);
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
		m_exited = true;
		m_updateCond.notify_one();
	}

	void SharedData::update()
	{
		//Clock clock;
		//clock.update();
		//m_updateTime = clock.getTime();

		m_status.frame += 1;
		Reader::ReadPlayer(m_status.player);
		m_status.itemCount = Reader::ReadItems(m_status.items);
		m_status.enemyCount = Reader::ReadEnemies(m_status.enemies);
		m_status.bulletCount = Reader::ReadBullets(m_status.bullets);
		m_status.laserCount = Reader::ReadLasers(m_status.lasers);
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
