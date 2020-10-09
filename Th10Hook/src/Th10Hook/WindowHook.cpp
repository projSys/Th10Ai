#include "Th10Hook/WindowHook.h"

#include "Th10Hook/DllLoader.h"

namespace th
{
#define WH_HOOK (WM_USER + 0x1234)
#define WH_UNHOOK (WM_USER + 0x1235)

	WindowHook g_windowHook;

	WindowHook::WindowHook() :
		Singleton(this),
		m_listener(nullptr),
		m_window(nullptr),
		m_unicode(true),
		m_prevWndProc(nullptr),
		m_hooked(true),
		m_unhooked(true),
		m_destroyed(false)
	{
	}

	void WindowHook::hook(HWND window, WindowListener* listener)
	{
		m_listener = listener;
		m_window = window;
		m_unicode = IsWindowUnicode(m_window);

		if (m_unicode)
			m_prevWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(m_window, GWLP_WNDPROC,
				reinterpret_cast<LONG_PTR>(&WindowHook::WindowProc)));
		else
			m_prevWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(m_window, GWLP_WNDPROC,
				reinterpret_cast<LONG_PTR>(&WindowHook::WindowProc)));
		if (m_prevWndProc == nullptr)
			BASE_THROW(WindowsError());

		//todo ”–Œ Ã‚
		m_hooked = false;
		if (m_unicode)
			SendMessageW(m_window, WH_HOOK, 0, 0);
		else
			SendMessageA(m_window, WH_HOOK, 0, 0);
		//if (!m_hooked)
		//	BASE_THROW(Exception(u8"π“‘ÿπ≥◊” ß∞‹°£"));
	}

	void WindowHook::unhook()
	{
		if (!m_destroyed)
		{
			m_unhooked = false;
			if (m_unicode)
				SendMessageW(m_window, WH_UNHOOK, 0, 0);
			else
				SendMessageA(m_window, WH_UNHOOK, 0, 0);
			//if (!m_unhooked)
			//	BASE_THROW(Exception(u8"–∂‘ÿπ≥◊” ß∞‹°£"));

			if (m_unicode)
				SetWindowLongPtrW(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevWndProc));
			else
				SetWindowLongPtrA(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevWndProc));
		}
	}

	bool WindowHook::isDestroyed() const
	{
		return m_destroyed;
	}

	LRESULT WindowHook::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowHook& windowHook = WindowHook::GetInstance();
		return windowHook.windowProc(window, message, wParam, lParam);
	}

	LRESULT WindowHook::windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		try
		{
			switch (message)
			{
			case WH_HOOK:
				if (!m_hooked)
				{
					m_listener->onHook();
					m_hooked = true;
				}
				break;

			case WH_UNHOOK:
				if (!m_unhooked)
				{
					m_listener->onUnhook();
					m_unhooked = true;
				}
				break;

			case WM_DESTROY:
			{
				m_destroyed = true;
				m_listener->onUnhook();
				m_listener->onDestroy();
				DllLoader& dllLoader = DllLoader::GetInstance();
				dllLoader.join();
				break;
			}
			}
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		if (m_unicode)
			return CallWindowProcW(m_prevWndProc, window, message, wParam, lParam);
		else
			return CallWindowProcA(m_prevWndProc, window, message, wParam, lParam);
	}
}
