#pragma once

#include <Base/Point.h>

#include "GraphCap/Input.h"

namespace gc
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
		static const WORD KEY_MAP[KEY_MAXCOUNT];
		static const int IS_KEY_PRESSED_MAP[KEY_MAXCOUNT];
		static const DWORD MOUSE_PRESS_MAP[MOUSE_MAXCOUNT];
		static const DWORD MOUSE_RELEASE_MAP[MOUSE_MAXCOUNT];
		static const int IS_MOUSE_PRESSED_MAP[MOUSE_MAXCOUNT];
	};
}
