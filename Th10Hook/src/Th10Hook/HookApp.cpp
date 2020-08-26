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

		m_windowHook.attach(m_shared->getWindow());
		ON_SCOPE_EXIT([&]()
			{
				if (isSelfExit())
					m_windowHook.detach();
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
		m_d3d9Hook = std::make_unique<D3D9Hook>(this);
		m_di8Hook = std::make_unique<DI8Hook>(this);
		m_hooked = true;
	}

	void HookApp::onUnhook()
	{
		m_di8Hook = nullptr;
		m_d3d9Hook = nullptr;
	}

	void HookApp::onDestroy()
	{
		setSelfExit(false);
		onUnhook();
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

	}
}
