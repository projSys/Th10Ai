#include "Th10Hook/HookDll.h"

#include <iostream>
#include <Base/ScopeGuard.h>

namespace th
{
	HookDll::HookDll()
	{
	}

	HookDll::~HookDll()
	{
	}

	void HookDll::run()
	{
		//AllocConsole();
		//freopen("conin$", "r", stdin);
		//freopen("conout$", "w", stdout);
		//freopen("conout$", "w", stderr);

		//HWND console = GetConsoleWindow();
		//HMENU menu = GetSystemMenu(console, FALSE);
		//EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

		m_shared = std::make_unique<SharedMemory>();
		ON_SCOPE_EXIT([&]()
			{
				m_shared = nullptr;
			});

		WindowHook& windowHook = WindowHook::GetInstance();
		windowHook.hook(m_shared->getWindow(), this);
		ON_SCOPE_EXIT([&]()
			{
				windowHook.unhook();
			});

		m_shared->notifyHook();
		m_shared->waitUnhook();
		m_shared->notifyExit();
	}

	void HookDll::onHook()
	{
		m_d3d9Hook = std::make_unique<D3D9Hook>(this);
		m_di8Hook = std::make_unique<DI8Hook>(this);
		m_th10Hook = std::make_unique<Th10Hook>(this);
	}

	void HookDll::onUnhook()
	{
		m_th10Hook = nullptr;
		m_di8Hook = nullptr;
		m_d3d9Hook = nullptr;
	}

	void HookDll::onDestroy()
	{
		m_shared->notifyUnhook();
	}

	void HookDll::onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		m_shared->update();
		m_shared->notifyUpdate();
	}

	void HookDll::onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data)
	{
		// c_dfDIKeyboard
		if (size == 256 && data != nullptr)
		{
			//lock_guard<mutex> lock(m_keyMutex);
			if (m_shared->isActionUpdated())
			{
				BYTE* keyState = reinterpret_cast<BYTE*>(data);

				if (m_shared->getAction().left)
					keyState[DIK_LEFT] = 0x80;
				else
					keyState[DIK_LEFT] = 0x00;

				if (m_shared->getAction().right)
					keyState[DIK_RIGHT] = 0x80;
				else
					keyState[DIK_RIGHT] = 0x00;

				if (m_shared->getAction().up)
					keyState[DIK_UP] = 0x80;
				else
					keyState[DIK_UP] = 0x00;

				if (m_shared->getAction().down)
					keyState[DIK_DOWN] = 0x80;
				else
					keyState[DIK_DOWN] = 0x00;

				if (m_shared->getAction().shoot)
					keyState[DIK_Z] = 0x80;
				else
					keyState[DIK_Z] = 0x00;

				if (m_shared->getAction().bomb)
					keyState[DIK_X] = 0x80;
				else
					keyState[DIK_X] = 0x00;

				if (m_shared->getAction().slow)
					keyState[DIK_LSHIFT] = 0x80;
				else
					keyState[DIK_LSHIFT] = 0x00;

				if (m_shared->getAction().skip)
					keyState[DIK_LCONTROL] = 0x80;
				else
					keyState[DIK_LCONTROL] = 0x00;

				//m_isKeyReadied = false;
			}
			else
			{
				std::cout << "ÊäÈëÌ«ÂýÁË¡£" << std::endl;
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
