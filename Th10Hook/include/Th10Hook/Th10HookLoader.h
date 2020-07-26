#pragma once

#include <memory>

#include "Th10Hook/DllLoader.h"
#include "Th10Hook/Th10Context.h"
#include "Th10Hook/WindowHook.h"
#include "Th10Hook/D3D9Hook.h"
#include "Th10Hook/DI8Hook.h"

namespace th
{
	class Th10HookLoader :
		public DllLoader,
		public WindowListener,
		public D3D9Listener,
		public DI8Listener
	{
	public:
		Th10HookLoader();

	private:
		virtual void run() override;
		virtual void onWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) override;
		void onLoad();
		void onUnload();
		void onDestroy();
		virtual void onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion) override;
		virtual void onGetDeviceStateW(IDirectInputDevice8W* device, DWORD size, LPVOID data) override;

		Logger m_logger;
		unique_ptr<Th10Context> m_context;
		WindowHook m_windowHook;
		unique_ptr<D3D9Hook> m_d3d9Hook;
		unique_ptr<DI8Hook> m_di8Hook;
	};
}
