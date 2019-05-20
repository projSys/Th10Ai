#include "TH10Bot/Common.h"
#include "TH10Bot/Mover.h"

namespace th
{
	Mover::Mover(Direction targetDir) :
		m_targetDir(targetDir),
		m_i(0)
	{
		if (m_targetDir == DIR_NONE)
			m_targetDir = DIR_HOLD;
	}

	bool Mover::hasNext() const
	{
		return m_i < MOVE_PRIORITY_END[m_targetDir];
	}

	Direction Mover::next()
	{
		Direction dir = MOVE_PRIORITY_DIR[m_targetDir][m_i];
		++m_i;
		return dir;
	}

	int_t Mover::getSize() const
	{
		return MOVE_PRIORITY_END[m_targetDir];
	}
}
