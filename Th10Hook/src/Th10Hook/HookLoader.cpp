#include "Th10Hook/HookLoader.h"

#include <Base/ScopeGuard.h>
#include <Windows/Apis.h>

#include "Th10Hook/DllMain.h"

namespace th
{
#define HL_LOAD (WM_USER + 0x1234)
#define HL_UNLOAD (WM_USER + 0x1235)

	HookLoader g_hookLoader;

	HookLoader::HookLoader() :
		m_windowHook(this),
		m_hooked(false),
		m_th10Hook(this)
	{
	}

	void HookLoader::run()
	{
		std::string logName = Apis::GetModuleDir(g_dll) + "/Th10Hook_%N.log";
		m_logger.addFileLog(logName);

		m_context = std::make_unique<Th10Context>();
		ON_SCOPE_EXIT([&]()
			{
				m_context = nullptr;
			});

		m_windowHook.attach(m_context->getWindow());
		ON_SCOPE_EXIT([&]()
			{
				if (isSelfExit())
					m_windowHook.detach();
			});
		m_windowHook.sendMessage(HL_LOAD, 0, 0);
		ON_SCOPE_EXIT([&]()
			{
				if (isSelfExit())
					m_windowHook.sendMessage(HL_UNLOAD, 0, 0);
			});
		if (!m_hooked)
			BASE_THROW(Exception());

		m_context->notifyHook();
		m_context->waitUnhook();
		m_context->notifyExit();
	}

	void HookLoader::onWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case HL_LOAD:
			onLoad();
			break;

		case HL_UNLOAD:
			onUnload();
			break;

		case WM_DESTROY:
			onDestroy();
			break;
		}
	}

	void HookLoader::onLoad()
	{
		m_d3d9Hook = std::make_unique<D3D9Hook>(this);
		m_di8Hook = std::make_unique<DI8Hook>(this);
		m_hooked = true;
	}

	void HookLoader::onUnload()
	{
		m_di8Hook = nullptr;
		m_d3d9Hook = nullptr;
	}

	void HookLoader::onDestroy()
	{
		setSelfExit(false);
		onUnload();
		m_context->notifyUnhook();
		join();
	}

	void HookLoader::onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		m_context->update();
		m_context->notifyUpdate();
	}

	void HookLoader::onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data)
	{

	}
}
