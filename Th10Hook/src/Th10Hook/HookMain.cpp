#include "Th10Hook/HookMain.h"

#include <iostream>
#include <Base/ScopeGuard.h>

namespace th
{
	HookMain::HookMain()
	{
		//AllocConsole();
		//freopen("conin$", "r", stdin);
		//freopen("conout$", "w", stdout);
		//freopen("conout$", "w", stderr);

		//HWND console = GetConsoleWindow();
		//HMENU menu = GetSystemMenu(console, FALSE);
		//EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

		m_sharedMemory = interprocess::managed_windows_shared_memory(
			interprocess::open_only, "Th10-SharedMemory");
		m_sharedData = m_sharedMemory.find<SharedData>("Th10-SharedData").first;
		if (m_sharedData == nullptr)
			BASE_THROW(Exception(u8"Th10-SharedData未找到。"));
	}

	HookMain::~HookMain()
	{
	}

	void HookMain::run()
	{
		// 会触发监听器的虚函数回调，不要放在构造函数
		WindowHook& windowHook = WindowHook::GetInstance();
		windowHook.hook(m_sharedData->getWindow(), this);
		ON_SCOPE_EXIT([&]()
			{
				windowHook.unhook();
			});

		m_sharedData->notifyHook();
		m_sharedData->waitUnhook();
		m_sharedData->notifyExit();
	}

	void HookMain::onHook()
	{
		m_d3d9Hook = std::make_unique<D3D9Hook>(this);
		m_di8Hook = std::make_unique<DI8Hook>(this);
		m_th10Hook = std::make_unique<Th10Hook>(this);
	}

	void HookMain::onUnhook()
	{
		m_th10Hook = nullptr;
		m_di8Hook = nullptr;
		m_d3d9Hook = nullptr;
	}

	void HookMain::onDestroy()
	{
		m_sharedData->notifyUnhook();
	}

	void HookMain::onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		m_sharedData->update();
		m_sharedData->notifyUpdate();
	}

	void HookMain::onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data)
	{
		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
			//lock_guard<mutex> lock(m_keyMutex);
			if (m_sharedData->isActionUpdated())
			{
				BYTE* keyState = reinterpret_cast<BYTE*>(data);

				if (m_sharedData->getAction().left)
					keyState[DIK_LEFT] = 0x80;
				else
					keyState[DIK_LEFT] = 0x00;

				if (m_sharedData->getAction().right)
					keyState[DIK_RIGHT] = 0x80;
				else
					keyState[DIK_RIGHT] = 0x00;

				if (m_sharedData->getAction().up)
					keyState[DIK_UP] = 0x80;
				else
					keyState[DIK_UP] = 0x00;

				if (m_sharedData->getAction().down)
					keyState[DIK_DOWN] = 0x80;
				else
					keyState[DIK_DOWN] = 0x00;

				if (m_sharedData->getAction().shoot)
					keyState[DIK_Z] = 0x80;
				else
					keyState[DIK_Z] = 0x00;

				if (m_sharedData->getAction().bomb)
					keyState[DIK_X] = 0x80;
				else
					keyState[DIK_X] = 0x00;

				if (m_sharedData->getAction().slow)
					keyState[DIK_LSHIFT] = 0x80;
				else
					keyState[DIK_LSHIFT] = 0x00;

				if (m_sharedData->getAction().skip)
					keyState[DIK_LCONTROL] = 0x80;
				else
					keyState[DIK_LCONTROL] = 0x00;

				//m_isKeyReadied = false;
			}
			else
			{
				std::cout << "输入太慢了。" << std::endl;
			}
		}
		// c_dfDIMouse
		//sizeof(DIMOUSESTATE);
		// c_dfDIMouse2
		//sizeof(DIMOUSESTATE2);
		// c_dfDIJoystick
		//sizeof(DIJOYSTATE);
		// c_dfDIJoystick2
		//sizeof(DIJOYSTATE2);
	}
}
