#pragma once

#include <vector>

#include "Windows/Point.h"
#include "Windows/Size.h"
#include "Windows/Rect.h"

namespace win
{
	class Window
	{
	public:
		static vector<Window> EnumWindows();
		static Window FindByClassName(const string& className);
		static Window FindByName(const string& name);

		Window();
		explicit Window(HWND window);
		~Window();
		Window(const Window&) = delete;
		Window(Window&& other);
		Window& operator =(const Window&) = delete;
		Window& operator =(Window&& other);
		void swap(Window& other);

		Window findChildByClassName(const string& className);
		Window findChildByName(const string& name);

		void minimize();
		void maximize();
		void restore();
		bool isMinimized() const;
		bool isMaximized() const;
		void activate();

		string getName() const;
		void setSize(const Size& size);
		Size getSize() const;
		Rect getRect() const;
		Rect getClientRect() const;
		DWORD getThreadId() const;
		DWORD getProcessId() const;

		operator HWND() const;

	private:
		HWND m_window;
	};
}
