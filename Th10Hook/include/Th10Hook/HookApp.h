#pragma once

#include "Th10Hook/Common.h"

#include <memory>

#include "Th10Hook/DllApp.h"
#include "Th10Hook/SharedMemory.h"
#include "Th10Hook/WindowHook.h"
#include "Th10Hook/D3D9Hook.h"
#include "Th10Hook/DI8Hook.h"
#include "Th10Hook/Th10Hook.h"

namespace th
{
	class HookApp :
		public DllApp,
		public WindowListener,
		public D3D9Listener,
		public DI8Listener,
		public Th10Listener
	{
	public:
		HookApp();

	private:
		virtual void run() override;
		virtual void onWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) override;
		void onHook();
		void onUnhook();
		void onDestroy();
		virtual void onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion) override;
		virtual void onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data) override;

		Logger m_logger;
		WindowHook m_windowHook;
		std::unique_ptr<SharedMemory> m_shared;
		std::unique_ptr<D3D9Hook> m_d3d9Hook;
		std::unique_ptr<DI8Hook> m_di8Hook;
		bool m_hooked;
		Th10Hook m_th10Hook;
	};
}
