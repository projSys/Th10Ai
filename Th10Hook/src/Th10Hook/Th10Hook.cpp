#include "Th10Hook/Th10Hook.h"

#include "Th10Hook/Th10Apis.h"

namespace th
{
	std::unique_ptr<Th10Hook> g_th10Hook;

	Th10Hook::Th10Hook(HWND window) :
		m_sharedMemory(bip::open_only, "Th10SharedMemory"),
		m_sharedData(nullptr)
	{
		m_sharedData = m_sharedMemory.find<SharedData>("Th10SharedData").first;
		if (m_sharedData == nullptr)
			throw Exception("Th10SharedData未找到。");

		m_sharedData->window = window;
		notifyInit();
	}

	Th10Hook::~Th10Hook()
	{
		notifyExit();
	}

	void Th10Hook::updateStatus()
	{
		if (!m_sharedData->active)
			return;

		SharedStatus& status = *(m_sharedData->writableStatus);
		Th10Apis::GetGlobalVarTo(status.globalVar);
		Th10Apis::GetPlayerTo(status.player);
		Th10Apis::GetItemsTo(status.items);
		Th10Apis::GetEnemiesTo(status.enemies);
		Th10Apis::GetBulletsTo(status.bullets);
		Th10Apis::GetLasersTo(status.lasers);

		notifyUpdate();
	}

	void Th10Hook::commitInputTo(DWORD size, LPVOID data)
	{
		if (!m_sharedData->active)
			return;

		if (!waitInput())
		{
			std::cout << "错误：等待输入超时。" << std::endl;
			return;
		}

		const SharedInput& input = *(m_sharedData->readableInput);
		input.commitTo(size, data);
	}

	void Th10Hook::notifyInit()
	{
		{
			//bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->initMutex);
			bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->statusMutex);
			m_sharedData->inited = true;
		}
		//m_sharedData->initCond.notify_one();
		m_sharedData->statusCond.notify_one();
	}

	void Th10Hook::notifyExit()
	{
		{
			bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->statusMutex);
			m_sharedData->exit = true;
		}
		m_sharedData->statusCond.notify_one();
	}

	void Th10Hook::notifyUpdate()
	{
		{
			bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->statusMutex);
			if (m_sharedData->statusUpdated)
				std::cout << "错误：处理太慢，状态跳帧了。" << std::endl;
			m_sharedData->writableStatus.swap(m_sharedData->swappableStatus);
			m_sharedData->statusUpdated = true;
		}
		m_sharedData->statusCond.notify_one();
	}

	bool Th10Hook::waitInput(const Time& timeout)
	{
		bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->inputMutex);
		if (!m_sharedData->inputUpdated)
			std::cout << "警告：处理太慢，等待输入。" << std::endl;
		while (!m_sharedData->inputUpdated)
		{
			bip::cv_status status = m_sharedData->inputCond.wait_for(lock, timeout);
			if (status == bip::cv_status::timeout)
				return false;
		}
		m_sharedData->readableInput.swap(m_sharedData->swappableInput);
		m_sharedData->inputUpdated = false;
		return true;
	}
}
