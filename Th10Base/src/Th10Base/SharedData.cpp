#include "Th10Base/SharedData.h"

#include <iostream>

namespace th
{
	SharedData::SharedData(SharedMemory& sharedMemory) :
		m_inited(false),
		m_active(false),
		m_window(nullptr),
		m_statusUpdated(false),
		m_exit(false),
		m_writableStatus(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(bip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		m_swappableStatus(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(bip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		m_readableStatus(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedStatus>(bip::anonymous_instance)(sharedMemory),
			sharedMemory)),
		m_inputUpdated(true),	// 跳过第一帧输入
		m_writableInput(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedInput>(bip::anonymous_instance)(),
			sharedMemory)),
		m_swappableInput(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedInput>(bip::anonymous_instance)(),
			sharedMemory)),
		m_readableInput(bip::make_managed_unique_ptr(
			sharedMemory.construct<SharedInput>(bip::anonymous_instance)(),
			sharedMemory))
	{
	}

	void SharedData::notifyInit()
	{
		{
			//bip::scoped_lock<bip::interprocess_mutex> lock(m_initMutex);
			bip::scoped_lock<bip::interprocess_mutex> lock(m_statusMutex);
			m_inited = true;
		}
		//m_initCond.notify_one();
		m_statusCond.notify_one();
	}

	bool SharedData::waitInit(const Time& timeout)
	{
		//bip::scoped_lock<bip::interprocess_mutex> lock(m_initMutex);
		bip::scoped_lock<bip::interprocess_mutex> lock(m_statusMutex);
		while (!m_inited)
		{
			//bip::cv_status status = m_initCond.wait_for(lock, timeout);
			bip::cv_status status = m_statusCond.wait_for(lock, timeout);
			if (status == bip::cv_status::timeout)
				return false;
		}
		return true;
	}

	bool SharedData::isActive() const
	{
		return m_active;
	}

	void SharedData::setActive(bool active)
	{
		m_active = active;
	}

	HWND SharedData::getWindow() const
	{
		return m_window;
	}

	void SharedData::setWindow(HWND window)
	{
		m_window = window;
	}

	void SharedData::notifyExit()
	{
		{
			bip::scoped_lock<bip::interprocess_mutex> lock(m_statusMutex);
			m_exit = true;
		}
		m_statusCond.notify_one();
	}

	void SharedData::notifyUpdate()
	{
		{
			bip::scoped_lock<bip::interprocess_mutex> lock(m_statusMutex);
			if (m_statusUpdated)
				std::cout << "错误：处理太慢，状态跳帧了。" << std::endl;
			m_writableStatus.swap(m_swappableStatus);
			m_statusUpdated = true;
		}
		m_statusCond.notify_one();
	}

	bool SharedData::waitUpdate()
	{
		bip::scoped_lock<bip::interprocess_mutex> lock(m_statusMutex);
		if (m_statusUpdated)
			std::cout << "警告：处理太慢，状态已更新。" << std::endl;
		while (!m_statusUpdated && !m_exit)
			m_statusCond.wait(lock);
		m_readableStatus.swap(m_swappableStatus);
		m_statusUpdated = false;
		return !m_exit;
	}

	bool SharedData::waitUpdate(const Time& timeout)
	{
		bip::scoped_lock<bip::interprocess_mutex> lock(m_statusMutex);
		if (m_statusUpdated)
			std::cout << "警告：处理太慢，状态已更新。" << std::endl;
		while (!m_statusUpdated && !m_exit)
		{
			bip::cv_status status = m_statusCond.wait_for(lock, timeout);
			if (status == bip::cv_status::timeout)
				return false;
		}
		m_readableStatus.swap(m_swappableStatus);
		m_statusUpdated = false;
		return true;
	}

	bool SharedData::isExit() const
	{
		return m_exit;
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
			bip::scoped_lock<bip::interprocess_mutex> lock(m_inputMutex);
			if (m_inputUpdated)
				std::cout << "错误：处理太慢，输入跳帧了。" << std::endl;
			m_writableInput.swap(m_swappableInput);
			m_inputUpdated = true;
		}
		m_inputCond.notify_one();
	}

	bool SharedData::waitInput(const Time& timeout)
	{
		bip::scoped_lock<bip::interprocess_mutex> lock(m_inputMutex);
		if (!m_inputUpdated)
			std::cout << "警告：处理太慢，等待输入。" << std::endl;
		while (!m_inputUpdated)
		{
			bip::cv_status status = m_inputCond.wait_for(lock, timeout);
			if (status == bip::cv_status::timeout)
				return false;
		}
		m_readableInput.swap(m_swappableInput);
		m_inputUpdated = false;
		return true;
	}

	SharedInput& SharedData::getWritableInput()
	{
		return *m_writableInput;
	}

	const SharedInput& SharedData::getReadableInput() const
	{
		return *m_readableInput;
	}
}
