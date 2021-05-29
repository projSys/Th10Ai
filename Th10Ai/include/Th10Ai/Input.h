#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Direction.h"

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

		void commit(DWORD size, LPVOID data);

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
