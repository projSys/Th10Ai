#pragma once

#include <array>
#include <Base/Point.h>

#include "libTH10AI/Input.h"

namespace th
{
	class DesktopInput
	{
	public:
		void keyPress(Key key);
		void keyRelease(Key key);
		bool isKeyPressed(Key key) const;

		void mousePress(Mouse mouse);
		void mouseRelease(Mouse mouse);
		bool isMousePressed(Mouse mouse) const;

		Point getMousePos() const;
		void setMousePos(const Point& pos);

	private:
		static const std::array<WORD, KEY_MAXCOUNT> KEY_MAP;
		static const std::array<int, KEY_MAXCOUNT> IS_KEY_PRESSED_MAP;
		static const std::array<DWORD, KEY_MAXCOUNT> MOUSE_PRESS_MAP;
		static const std::array<DWORD, KEY_MAXCOUNT> MOUSE_RELEASE_MAP;
		static const std::array<int, KEY_MAXCOUNT> IS_MOUSE_PRESSED_MAP;
	};
}
