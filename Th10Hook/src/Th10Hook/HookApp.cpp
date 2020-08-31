#include "Th10Hook/HookApp.h"

#include <Base/ScopeGuard.h>
#include <Windows/Apis.h>

#include "Th10Hook/DllMain.h"

namespace th
{
#define HA_HOOK (WM_USER + 0x1234)
#define HA_UNHOOK (WM_USER + 0x1235)

	HookApp g_hookApp;

	HookApp::HookApp() :
		m_windowHook(this),
		m_hooked(false),
		m_th10Hook(this)
	{
	}

	void HookApp::run()
	{
		std::string logName = Apis::GetModuleDir(g_dll) + "/Th10Hook_%N.log";
		m_logger.addFileLog(logName);

		m_shared = std::make_unique<SharedMemory>();
		ON_SCOPE_EXIT([&]()
			{
				m_shared = nullptr;
			});

		m_windowHook.hook(m_shared->getWindow());
		ON_SCOPE_EXIT([&]()
			{
				if (isSelfExit())
					m_windowHook.unhook();
			});
		m_windowHook.sendMessage(HA_HOOK, 0, 0);
		ON_SCOPE_EXIT([&]()
			{
				if (isSelfExit())
					m_windowHook.sendMessage(HA_UNHOOK, 0, 0);
			});
		if (!m_hooked)
			BASE_THROW(Exception());

		m_shared->notifyHook();
		m_shared->waitUnhook();
		m_shared->notifyExit();
	}

	void HookApp::onWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case HA_HOOK:
			onHook();
			break;

		case HA_UNHOOK:
			onUnhook();
			break;

		case WM_DESTROY:
			onDestroy();
			break;
		}
	}

	void HookApp::onHook()
	{
		if (m_hooked)
			return;

		m_d3d9Hook = std::make_unique<D3D9Hook>(this);
		m_di8Hook = std::make_unique<DI8Hook>(this);
		m_hooked = true;
	}

	void HookApp::onUnhook()
	{
		m_hooked = false;
		m_di8Hook = nullptr;
		m_d3d9Hook = nullptr;
	}

	void HookApp::onDestroy()
	{
		onUnhook();
		setSelfExit(false);
		m_shared->notifyUnhook();
		join();
	}

	void HookApp::onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		m_shared->update();
		m_shared->notifyUpdate();
	}

	void HookApp::onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data)
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
				//cout << "输入不及时。" << endl;
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
