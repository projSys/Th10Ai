#include "TH10Bot/Common.h"
#include "TH10Bot/Mover.h"

namespace th
{
	Mover::Mover(Direction fromDir) :
		m_fromDir(fromDir),
		m_i(0)
	{
		if (m_fromDir == DIR_NONE)
			m_fromDir = DIR_HOLD;
	}

	bool Mover::hasNext() const
	{
		return m_i < FROM_END[m_fromDir];
	}

	Direction Mover::next()
	{
		Direction dir = FROM_PRIORITY[m_fromDir][m_i];
		++m_i;
		return dir;
	}
}
