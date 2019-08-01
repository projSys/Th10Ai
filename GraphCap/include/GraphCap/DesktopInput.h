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

		void mouseLeftPress();
		void mouseLeftRelease();
		void mouseRightPress();
		void mouseRightRelease();

		Point getMousePos() const;
		void setMousePos(const Point& pos);

	private:
		static const WORD KEY_MAP1[KEY_MAXCOUNT];
		static const WORD KEY_MAP2[KEY_MAXCOUNT];
	};
}
