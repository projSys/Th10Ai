#pragma once

#include "Th10Base/Common.h"

#include "Th10Base/Direction.h"

namespace th
{
	class SharedInput
	{
	public:
		SharedInput();

		void clear();

		void move(Direction dir);
		void shoot();
		void bomb();
		void slow();
		void skip();

		void commitTo(DWORD size, LPVOID data) const;

	private:
		Direction m_dir;
		bool m_shoot;
		bool m_bomb;
		bool m_slow;
		bool m_skip;
	};
}
