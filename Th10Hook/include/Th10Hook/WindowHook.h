#pragma once

#include "Th10Hook/Common.h"

#include <Base/Singleton.h>

namespace th
{
	class WindowListener
	{
	public:
		virtual ~WindowListener() = default;
		virtual void onWindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) = 0;
	};

	class WindowHook :
		public Singleton<WindowHook>
	{
	public:
		WindowHook(WindowListener* listener);

		void hook(HWND window);
		void unhook();

		void sendMessage(UINT message, WPARAM wparam, LPARAM lparam);
		void postMessage(UINT message, WPARAM wparam, LPARAM lparam);

	protected:
		static LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
		LRESULT windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

		WindowListener* m_listener;
		HWND m_window;
		bool m_unicode;
		WNDPROC m_prevWndProc;
	};
}
