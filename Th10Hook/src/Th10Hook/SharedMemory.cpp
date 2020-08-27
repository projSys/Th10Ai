#include "Th10Hook/SharedMemory.h"

#include <Base/Clock.h>

#include "Th10Hook/Reader.h"

namespace th
{
	SharedMemory::SharedMemory() :
		m_memory(interprocess::open_only, "Th10-SharedMemory"),
		m_data(nullptr)
	{
		m_data = m_memory.find<SharedData>("Th10-SharedData").first;
		if (m_data == nullptr)
			BASE_THROW(Exception(u8"Th10-SharedDataÎ´ÕÒµ½¡£"));
	}

	SharedMemory::~SharedMemory()
	{
	}

	HWND SharedMemory::getWindow() const
	{
		return m_data->window;
	}

	void SharedMemory::notifyHook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->hookMutex);
		m_data->hooked = true;
		m_data->hookCond.notify_one();
	}

	void SharedMemory::notifyUnhook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->unhookMutex);
		m_data->unhooked = true;
		m_data->unhookCond.notify_one();
	}

	void SharedMemory::waitUnhook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->unhookMutex);
		if (!m_data->unhooked)
			m_data->unhookCond.wait(lock);
	}

	void SharedMemory::notifyUpdate()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->updateMutex);
		m_data->updated = true;
		m_data->updateCond.notify_one();
	}

	void SharedMemory::notifyExit()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->updateMutex);
		m_data->exited = true;
		m_data->updateCond.notify_one();
	}

	void SharedMemory::update()
	{
		//Clock clock;
		//clock.update();
		//m_data->updateTime = clock.getTime();

		Reader::ReadPlayer(m_data->status.player);
		m_data->status.itemCount = Reader::ReadItems(m_data->status.items);
		m_data->status.enemyCount = Reader::ReadEnemies(m_data->status.enemies);
		m_data->status.bulletCount = Reader::ReadBullets(m_data->status.bullets);
		m_data->status.laserCount = Reader::ReadLasers(m_data->status.lasers);
	}

	ActionData& SharedMemory::getAction()
	{
		return m_data->action;
	}

	bool SharedMemory::isActionUpdate() const
	{
		bool ret = m_data->actionUpdated;
		m_data->actionUpdated = false;
		return ret;
	}
}
