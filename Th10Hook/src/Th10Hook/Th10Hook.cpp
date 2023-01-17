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

		m_sharedData->setWindow(window);
		m_sharedData->notifyInit();
	}

	Th10Hook::~Th10Hook()
	{
		m_sharedData->notifyExit();
	}

	void Th10Hook::updateStatus()
	{
		if (!m_sharedData->isActive())
			return;

		SharedStatus& status = m_sharedData->getWritableStatus();
		Th10Apis::GetGlobalVarTo(status.globalVar);
		Th10Apis::GetPlayerTo(status.player);
		Th10Apis::GetItemsTo(status.items);
		Th10Apis::GetEnemiesTo(status.enemies);
		Th10Apis::GetBulletsTo(status.bullets);
		Th10Apis::GetLasersTo(status.lasers);

		m_sharedData->notifyUpdate();
	}

	void Th10Hook::commitInputTo(DWORD size, LPVOID data)
	{
		if (!m_sharedData->isActive())
			return;

		if (!m_sharedData->waitInput(Time(8)))
		{
			std::cout << "错误：等待输入超时。" << std::endl;
			return;
		}

		const SharedInput& input = m_sharedData->getReadableInput();
		input.commitTo(size, data);
	}
}
