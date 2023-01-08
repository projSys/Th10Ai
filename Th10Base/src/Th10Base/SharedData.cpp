#include "Th10Base/SharedData.h"

namespace th
{
	SharedData::SharedData(SharedMemory& sharedMemory) :
		m_inited(false),
		m_window(nullptr),
		m_active(false),
		m_statusUpdated(false),
		m_exit(false),
		m_inputUpdated(true),	// 跳过第一帧输入
		m_writableStatus(ip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>("WritableStatus")(sharedMemory),
			sharedMemory)),
		m_swappableStatus(ip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>("SwappableStatus")(sharedMemory),
			sharedMemory)),
		m_readableStatus(ip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>("ReadableStatus")(sharedMemory),
			sharedMemory)),
		m_writableInput(ip::make_managed_unique_ptr(
			sharedMemory.construct<Input>("WritableInput")(),
			sharedMemory)),
		m_swappableInput(ip::make_managed_unique_ptr(
			sharedMemory.construct<Input>("SwappableInput")(),
			sharedMemory)),
		m_readableInput(ip::make_managed_unique_ptr(
			sharedMemory.construct<Input>("ReadableInput")(),
			sharedMemory))
	{
	}

	void SharedData::notifyInit()
	{
		{
			ip::scoped_lock<ip::interprocess_mutex> lock(m_initMutex);
			m_inited = true;
		}
		m_initCond.notify_one();
	}

	bool SharedData::waitInit(const Time& timeout)
	{
		ip::scoped_lock<ip::interprocess_mutex> lock(m_initMutex);
		while (!m_inited)
		{
			ip::cv_status status = m_initCond.wait_for(lock, timeout);
			if (status == ip::cv_status::timeout)
				return false;
		}
		return true;
	}

	HWND SharedData::getWindow() const
	{
		return m_window;
	}

	void SharedData::setWindow(HWND window)
	{
		m_window = window;
	}

	bool SharedData::isActive() const
	{
		return m_active;
	}

	void SharedData::setActive(bool active)
	{
		m_active = active;
	}

	void SharedData::notifyExit()
	{
		{
			ip::scoped_lock<ip::interprocess_mutex> lock(m_statusMutex);
			m_exit = true;
		}
		m_statusCond.notify_one();
	}

	void SharedData::notifyUpdate()
	{
		{
			ip::scoped_lock<ip::interprocess_mutex> lock(m_statusMutex);
			//if (m_statusUpdated)
			//	std::cout << "处理太慢，状态跳帧了。" << std::endl;
			m_writableStatus.swap(m_swappableStatus);
			m_statusUpdated = true;
		}
		m_statusCond.notify_one();
	}

	bool SharedData::waitUpdate()
	{
		ip::scoped_lock<ip::interprocess_mutex> lock(m_statusMutex);
		//if (m_statusUpdated)
		//	std::cout << "处理太慢，状态已更新。" << std::endl;
		while (!m_statusUpdated && !m_exit)
			m_statusCond.wait(lock);
		m_readableStatus.swap(m_swappableStatus);
		m_statusUpdated = false;
		return !m_exit;
	}

	SharedStatus& SharedData::getWritableStatus()
	{
		return *m_writableStatus;
	}

	const SharedStatus& SharedData::getReadableStatus() const
	{
		return *m_readableStatus;
	}

	void SharedData::notifyInput()
	{
		{
			ip::scoped_lock<ip::interprocess_mutex> lock(m_inputMutex);
			//if (m_inputUpdated)
			//	std::cout << "处理太快，输入跳帧了？" << std::endl;
			m_writableInput.swap(m_swappableInput);
			m_inputUpdated = true;
		}
		m_inputCond.notify_one();
	}

	bool SharedData::waitInput(const Time& timeout)
	{
		ip::scoped_lock<ip::interprocess_mutex> lock(m_inputMutex);
		//if (!m_inputUpdated)
		//	std::cout << "处理太慢，等待输入。" << std::endl;
		while (!m_inputUpdated)
		{
			ip::cv_status status = m_inputCond.wait_for(lock, timeout);
			if (status == ip::cv_status::timeout)
				return false;
		}
		m_readableInput.swap(m_swappableInput);
		m_inputUpdated = false;
		return true;
	}

	Input& SharedData::getWritableInput()
	{
		return *m_writableInput;
	}

	const Input& SharedData::getReadableInput() const
	{
		return *m_readableInput;
	}
}
