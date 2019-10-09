#pragma once

#include <vector>

#include "Base/Point.h"
#include "Base/Size.h"
#include "Base/Rect.h"

namespace win
{
	class Window
	{
	public:
		static std::vector<Window> EnumWindows();
		static Window FindByClassName(const std::string& className);
		static Window FindByName(const std::string& name);

		Window();
		explicit Window(HWND window);
		Window(const Window&) = delete;
		Window(Window&& other);
		~Window();
		Window& operator =(const Window&) = delete;
		Window& operator =(Window&& other);
		void swap(Window& other);

		Window findChildByClassName(const std::string& className);
		Window findChildByName(const std::string& name);

		void minimize();
		void maximize();
		void restore();
		bool isMinimized() const;
		bool isMaximized() const;
		void activate();

		std::string getName() const;
		Size getSize() const;
		void setSize(const Size& size);
		Rect getRect() const;
		Rect getClientRect() const;
		DWORD getThreadId() const;
		DWORD getProcessId() const;

		operator HWND() const;

	private:
		HWND m_window;
	};
}
