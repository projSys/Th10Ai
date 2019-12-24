#include "Windows/Common.h"
#include "Windows/Window.h"

#include <boost/locale.hpp>

#include "Windows/WindowsError.h"
#include "Windows/Util.h"

namespace win
{
	namespace blc = boost::locale::conv;

	std::vector<Window> Window::EnumWindows()
	{
		std::vector<Window> windows;
		auto enumFunc = [](HWND window, LPARAM lParam)->BOOL
		{
			std::vector<Window>* windows = reinterpret_cast<std::vector<Window>*>(lParam);
			if (IsWindowVisible(window))
				windows->push_back(Window(window));
			return TRUE;
		};
		if (!::EnumWindows(enumFunc, reinterpret_cast<LPARAM>(&windows)))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		return windows;
	}

	Window Window::FindByClassName(const std::string& className)
	{
		std::wstring classNameW = blc::utf_to_utf<wchar_t>(className);

		HWND window = FindWindowW(classNameW.c_str(), nullptr);
		if (window == nullptr)
			BASE_THROW_EXCEPTION(Exception("Class not found: " + className));
		return Window(window);
	}

	Window Window::FindByName(const std::string& name)
	{
		std::wstring nameW = blc::utf_to_utf<wchar_t>(name);

		HWND window = FindWindowW(nullptr, nameW.c_str());
		if (window == nullptr)
			BASE_THROW_EXCEPTION(Exception("Window not found: " + name));
		return Window(window);
	}

	Window::Window() :
		m_window(nullptr)
	{
	}

	Window::Window(HWND window) :
		m_window(window)
	{
	}

	Window::~Window()
	{
	}

	Window::Window(Window&& other) :
		m_window(other.m_window)
	{
		other.m_window = nullptr;
	}

	Window& Window::operator =(Window&& other)
	{
		Window(std::move(other)).swap(*this);
		return *this;
	}

	void Window::swap(Window& other)
	{
		std::swap(m_window, other.m_window);
	}

	Window Window::findChildByClassName(const std::string& className)
	{
		std::wstring classNameW = blc::utf_to_utf<wchar_t>(className);

		HWND window = FindWindowExW(m_window, nullptr, classNameW.c_str(), nullptr);
		if (window == nullptr)
			BASE_THROW_EXCEPTION(Exception("Class not found: " + className));
		return Window(window);
	}

	Window Window::findChildByName(const std::string& name)
	{
		std::wstring nameW = blc::utf_to_utf<wchar_t>(name);

		HWND window = FindWindowExW(m_window, nullptr, nullptr, nameW.c_str());
		if (window == nullptr)
			BASE_THROW_EXCEPTION(Exception("Window not found: " + name));
		return Window(window);
	}

	void Window::minimize()
	{
		if (!ShowWindow(m_window, SW_MINIMIZE))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
	}

	void Window::maximize()
	{
		if (!ShowWindow(m_window, SW_MAXIMIZE))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
	}

	void Window::restore()
	{
		if (!ShowWindow(m_window, SW_RESTORE))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
	}

	bool Window::isMinimized() const
	{
		return IsIconic(m_window);
	}

	bool Window::isMaximized() const
	{
		return IsZoomed(m_window);
	}

	void Window::activate()
	{
		if (!SetForegroundWindow(m_window))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
	}

	std::string Window::getName() const
	{
		TCHAR buffer[1024] = {};
		GetWindowText(m_window, buffer, 1023);
		return blc::utf_to_utf<char>(buffer);
	}

	Size Window::getSize() const
	{
		RECT rect = {};
		if (!GetWindowRect(m_window, &rect))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		return Size(rect.right - rect.left, rect.bottom - rect.top);
	}

	void Window::setSize(const Size& size)
	{
		if (!SetWindowPos(m_window, nullptr, 0, 0, static_cast<int>(size.width),
			static_cast<int>(size.height), SWP_NOMOVE | SWP_NOZORDER))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
	}

	Rect Window::getRect() const
	{
		RECT rect = {};
		if (!GetWindowRect(m_window, &rect))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		return Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	}

	Rect Window::getClientRect() const
	{
		RECT rect = {};
		if (!GetClientRect(m_window, &rect))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		POINT leftTop = { rect.left, rect.top };
		if (!ClientToScreen(m_window, &leftTop))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		POINT rightBottom = { rect.right, rect.bottom };
		if (!ClientToScreen(m_window, &rightBottom))
			BASE_THROW_EXCEPTION(WindowsError(GetLastError()));
		return Rect(leftTop.x, leftTop.y, rightBottom.x - leftTop.x, rightBottom.y - leftTop.y);
	}

	DWORD Window::getThreadId() const
	{
		return GetWindowThreadProcessId(m_window, nullptr);
	}

	DWORD Window::getProcessId() const
	{
		DWORD processId = 0;
		GetWindowThreadProcessId(m_window, &processId);
		return processId;
	}

	Window::operator HWND() const
	{
		return m_window;
	}
}
