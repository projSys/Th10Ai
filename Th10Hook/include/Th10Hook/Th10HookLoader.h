#pragma once

#include <memory>

#include "Th10Hook/DllLoader.h"
#include "Th10Hook/WindowHook.h"
#include "Th10Hook/Th10Context.h"
#include "Th10Hook/D3D9Hook.h"
#include "Th10Hook/DI8Hook.h"
#include "Th10Hook/Reader2.h"

namespace th
{
	class Th10Ai;

	class Th10HookLoader :
		public DllLoader,
		public WindowListener
	{
	public:
		Th10HookLoader();

	private:
		virtual void run() override;
		virtual void onWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) override;
		void onLoad();
		void onUnload();
		void onDestroy();

		WindowHook m_windowHook;

		Logger m_logger;

		unique_ptr<Th10Ai> m_th10Ai;
		unique_ptr<Th10Context> m_context;
		unique_ptr<D3D9Hook> m_d3d9Hook;
		unique_ptr<DI8Hook> m_di8Hook;
	};
}
