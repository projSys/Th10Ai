#pragma once

#include <Base/Window.h>

namespace gc
{
	const int_t GC_D3D9FRAMESYNC = 0x01;
	const int_t GC_D3D9HOOK = 0x02;
	const int_t GC_D3D10HOOK = 0x04;
	const int_t GC_D3D11HOOK = 0x08;
	const int_t GC_DI8HOOK = 0x10;

	class GraphCap
	{
	public:
		GraphCap(Window& target, int_t capType);
		~GraphCap();

	private:
		typedef bool(WINAPI *Hook_t)(HWND, int_t);
		typedef void(WINAPI *Unhook_t)();

		HMODULE m_hookDll;
		Hook_t m_hookFunc;
		Unhook_t m_unhookFunc;
	};
}
