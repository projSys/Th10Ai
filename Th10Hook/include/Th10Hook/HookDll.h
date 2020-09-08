#pragma once

#include "Th10Hook/Common.h"

#include <memory>

#include "Th10Hook/SharedMemory.h"
#include "Th10Hook/WindowHook.h"
#include "Th10Hook/D3D9Hook.h"
#include "Th10Hook/DI8Hook.h"
#include "Th10Hook/Th10Hook.h"

namespace th
{
	class HookDll :
		public WindowListener,
		public D3D9Listener,
		public DI8Listener,
		public Th10Listener
	{
	public:
		HookDll();
		~HookDll();

		void run();

	private:
		virtual void onHook() override;
		virtual void onUnhook() override;
		virtual void onDestroy() override;
		virtual void onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion) override;
		virtual void onGetDeviceStateA(IDirectInputDevice8A* device, DWORD size, LPVOID data) override;

		std::unique_ptr<SharedMemory> m_shared;
		std::unique_ptr<D3D9Hook> m_d3d9Hook;
		std::unique_ptr<DI8Hook> m_di8Hook;
		std::unique_ptr<Th10Hook> m_th10Hook;
	};
}
