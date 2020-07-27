#include "Th10Hook/Common.h"
#include "Th10Hook/Th10HookLoader.h"

#include <Base/ScopeGuard.h>

#include "Th10Hook/DllMain.h"

namespace th
{
#define TH10HL_LOAD (WM_USER + 0x1234)
#define TH10HL_UNLOAD (WM_USER + 0x1235)

	Th10HookLoader g_th10HookLoader;

	Th10HookLoader::Th10HookLoader() :
		m_windowHook(this)
	{
	}

	void Th10HookLoader::run()
	{
		string logName = Apis::GetModuleDir(g_dll) + "/Th10Hook_%N.log";
		m_logger.addFileLog(logName);

		m_context = make_unique<Th10Context>();
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
		m_windowHook.sendMessage(TH10HL_LOAD, 0, 0);
		ON_SCOPE_EXIT([&]()
			{
				if (isSelfExit())
					m_windowHook.sendMessage(TH10HL_UNLOAD, 0, 0);
			});

		m_context->waitUnhook();
	}

	void Th10HookLoader::onWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		switch (message)
		{
		case TH10HL_LOAD:
			onLoad();
			break;

		case TH10HL_UNLOAD:
			onUnload();
			break;

		case WM_DESTROY:
			onDestroy();
			break;
		}
	}

	void Th10HookLoader::onLoad()
	{
		m_d3d9Hook = make_unique<D3D9Hook>(this);
		m_di8Hook = make_unique<DI8Hook>(this);
		m_context->notifyHook();
	}

	void Th10HookLoader::onUnload()
	{
		m_di8Hook = nullptr;
		m_d3d9Hook = nullptr;
	}

	void Th10HookLoader::onDestroy()
	{
		setSelfExit(false);

		onUnload();
		m_context->notifyUnhook();

		join();
	}

	void Th10HookLoader::onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		m_context->update();
		m_context->notifyUpdate();
	}

	void Th10HookLoader::onGetDeviceStateW(IDirectInputDevice8W* device, DWORD size, LPVOID data)
	{

	}
}
