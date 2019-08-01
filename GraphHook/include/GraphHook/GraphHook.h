#pragma once

#include <memory>
#include <Base/Singleton.h>

#include "GraphHook/HookIniter.h"
#include "GraphHook/D3D9FrameSync.h"
#include "GraphHook/D3D9Hook.h"
#include "GraphHook/D3D10Hook.h"
#include "GraphHook/D3D11Hook.h"
#include "GraphHook/DI8Hook.h"

namespace gh
{
#define WM_HOOK (WM_USER + 0x1234)
#define WM_UNHOOK (WM_USER + 0x1235)

	const int_t GH_D3D9FRAMESYNC = 0x01;
	const int_t GH_D3D9HOOK = 0x02;
	const int_t GH_D3D10HOOK = 0x04;
	const int_t GH_D3D11HOOK = 0x08;
	const int_t GH_DI8HOOK = 0x10;

	class GraphHook :
		public Singleton<GraphHook>
	{
	public:
		GraphHook();

		bool hook(HWND window, int_t hookType);
		void unhook();

	private:
		static LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam);
		LRESULT hookProc(int code, WPARAM wParam, LPARAM lParam);
		void onHook(int_t hookType);
		void onUnhook();

		std::shared_ptr<HookIniter> m_hookIniter;
		std::shared_ptr<D3D9FrameSync> m_d3d9FrameSync;
		std::shared_ptr<D3D9Hook> m_d3d9Hook;
		std::shared_ptr<D3D10Hook> m_d3d10Hook;
		std::shared_ptr<D3D11Hook> m_d3d11Hook;
		std::shared_ptr<DI8Hook> m_di8Hook;
	};
}
