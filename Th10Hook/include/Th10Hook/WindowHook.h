#pragma once

#include "Th10Hook/Common.h"

#include <atomic>
#include <Base/Singleton.h>

namespace th
{
	class WindowListener
	{
	public:
		virtual ~WindowListener() = default;
		virtual void onHook() = 0;
		virtual void onUnhook() = 0;
		virtual void onDestroy() = 0;
	};

	class WindowHook :
		public Singleton<WindowHook>
	{
	public:
		WindowHook();

		void hook(HWND window, WindowListener* listener);
		void unhook();

		bool isDestroyed() const;

	private:
		static LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

		WindowListener* m_listener;
		HWND m_window;
		bool m_unicode;
		WNDPROC m_prevWndProc;
		bool m_hooked;
		bool m_unhooked;
		std::atomic<bool> m_destroyed;
	};
}
