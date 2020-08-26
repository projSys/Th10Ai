#include "Th10Hook/WindowHook.h"

namespace th
{
	WindowHook::WindowHook(WindowListener* listener) :
		Singleton(this),
		m_listener(listener),
		m_window(nullptr),
		m_unicode(true),
		m_prevWndProc(nullptr)
	{
	}

	void WindowHook::attach(HWND window)
	{
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
	}

	void WindowHook::detach()
	{
		if (m_unicode)
			SetWindowLongPtrW(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevWndProc));
		else
			SetWindowLongPtrA(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_prevWndProc));
	}

	void WindowHook::sendMessage(UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (m_unicode)
			SendMessageW(m_window, message, wparam, lparam);
		else
			SendMessageA(m_window, message, wparam, lparam);
	}

	void WindowHook::postMessage(UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (m_unicode)
			PostMessageW(m_window, message, wparam, lparam);
		else
			PostMessageA(m_window, message, wparam, lparam);
	}

	LRESULT WindowHook::WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		WindowHook& windowHook = WindowHook::GetInstance();
		return windowHook.windowProc(window, message, wparam, lparam);
	}

	LRESULT WindowHook::windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
		try
		{
			m_listener->onWindowProc(window, message, wparam, lparam);
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		if (m_unicode)
			return CallWindowProcW(m_prevWndProc, window, message, wparam, lparam);
		else
			return CallWindowProcA(m_prevWndProc, window, message, wparam, lparam);
	}
}
