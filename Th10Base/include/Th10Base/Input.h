#pragma once

#include "Th10Base/Common.h"

#include "Th10Base/Direction.h"

namespace th
{
	class Input
	{
	public:
		Input();

		void clear();

		void move(DIR dir);
		void shoot();
		void bomb();
		void slow();
		void skip();

		void commitTo(DWORD size, LPVOID data) const;

	private:
		bool m_up;
		bool m_down;
		bool m_left;
		bool m_right;
		bool m_shoot;
		bool m_bomb;
		bool m_slow;
		bool m_skip;
	};
}
