#include "Th10Hook/Th10Hook.h"

#include <Base/Exception/Exception.h>
#include <Base/Windows/Apis.h>

#include "Th10Hook/Th10Apis.h"

namespace th
{
	std::unique_ptr<Th10Hook> g_th10Hook;

	Th10Hook::Th10Hook(HWND window) :
		m_sharedMemory(bip::open_only, "Th10SharedMemory"),
		m_sharedData(nullptr)
	{
		SetForegroundWindow(window);

		RECT rect = {};
		GetWindowRect(window, &rect);
		int windowWidth = rect.right - rect.left;
		int windowHeight = rect.bottom - rect.top;
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int x = (screenWidth - windowWidth) / 2;
		int y = (screenHeight - windowHeight) / 2;
		SetWindowPos(window, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		char buf1[1024] = {};
		GetWindowTextA(window, buf1, 1023);
		std::wstring buf2 = Apis::MultiByteToWideChar(932, buf1);	// Shift-JIS
		std::string buf3 = Apis::WideToAnsi(buf2);
		SetWindowTextA(window, buf3.c_str());

		//ULONG_PTR icon = GetClassLongPtrA(window, GCLP_HICON);
		HICON icon = LoadIconA(GetModuleHandleA(nullptr), "IDI_ICON3");
		SendMessageA(window, WM_SETICON, ICON_SMALL, (LPARAM)icon);

		m_sharedData = m_sharedMemory.find<SharedData>("Th10SharedData").first;
		if (m_sharedData == nullptr)
			Throw(Exception("Th10SharedData未找到。"));

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

		//static Time t0 = Clock::Now();
		//static int_t c0 = 0;
		//c0++;
		//if (Clock::Now() - t0 > Time(1000))
		//{
		//	std::cout << c0 << std::endl;
		//	t0 += Time(1000);
		//	c0 = 0;
		//}

		m_sharedData->notifyUpdate();
	}

	void Th10Hook::commitInputTo(DWORD size, LPVOID data)
	{
		if (!m_sharedData->isActive())
			return;

		//m_sharedData->waitInput();
		if (!m_sharedData->waitInput(Time(8)))
		{
			std::cout << "错误：等待输入超时。" << std::endl;
			return;
		}

		const SharedInput& input = m_sharedData->getReadableInput();
		input.commitTo(size, data);

		//static Time t0 = Clock::Now();
		//static int_t c0 = 0;
		//c0++;
		//if (Clock::Now() - t0 > Time(1000))
		//{
		//	std::cout << c0 << std::endl;
		//	t0 += Time(1000);
		//	c0 = 0;
		//}
	}
}
