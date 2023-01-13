#include "Th10Ai/Th10Hook.h"

namespace th
{
	Th10Hook::Th10Hook() :
		m_sharedMemory(ip::create_only, "Th10SharedMemory", 64 * 1024 * 1024),
		m_sharedData(nullptr)
	{
		m_sharedData = m_sharedMemory.construct<SharedData>("Th10SharedData")(m_sharedMemory);
		if (m_sharedData == nullptr)
			throw Exception("Th10SharedData名称已被使用。");
	}

	Th10Hook::~Th10Hook()
	{
		m_sharedMemory.destroy_ptr(m_sharedData);
	}

	bool Th10Hook::waitInit(const Time& timeout)
	{
		//ip::scoped_lock<ip::interprocess_mutex> lock(m_sharedData->initMutex);
		ip::scoped_lock<ip::interprocess_mutex> lock(m_sharedData->statusMutex);
		while (!m_sharedData->inited)
		{
			//ip::cv_status status = m_sharedData->initCond.wait_for(lock, timeout);
			ip::cv_status status = m_sharedData->statusCond.wait_for(lock, timeout);
			if (status == ip::cv_status::timeout)
				return false;
		}
		return true;
	}

	bool Th10Hook::isActive() const
	{
		return m_sharedData->active;
	}

	void Th10Hook::setActive(bool active)
	{
		m_sharedData->active = active;
	}

	HWND Th10Hook::getWindow() const
	{
		return m_sharedData->window;
	}

	bool Th10Hook::waitUpdate()
	{
		ip::scoped_lock<ip::interprocess_mutex> lock(m_sharedData->statusMutex);
		if (m_sharedData->statusUpdated)
			std::cout << "警告：处理太慢，状态已更新。" << std::endl;
		while (!m_sharedData->statusUpdated && !m_sharedData->exit)
			m_sharedData->statusCond.wait(lock);
		m_sharedData->readableStatus.swap(m_sharedData->swappableStatus);
		m_sharedData->statusUpdated = false;
		return !m_sharedData->exit;
	}

	const SharedStatus& Th10Hook::getReadableStatus() const
	{
		return *(m_sharedData->readableStatus);
	}

	void Th10Hook::notifyInput()
	{
		{
			ip::scoped_lock<ip::interprocess_mutex> lock(m_sharedData->inputMutex);
			if (m_sharedData->inputUpdated)
				std::cout << "错误：处理太慢，输入跳帧了。" << std::endl;
			m_sharedData->writableInput.swap(m_sharedData->swappableInput);
			m_sharedData->inputUpdated = true;
		}
		m_sharedData->inputCond.notify_one();
	}

	Input& Th10Hook::getWritableInput()
	{
		return *(m_sharedData->writableInput);
	}
}
