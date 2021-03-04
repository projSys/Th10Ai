#include "Th10Hook/Enemy.h"

namespace th
{
	Enemy::Enemy()
	{
	}

	bool Enemy::isBoss() const
	{
		return getSize() == Sizef(32.0, 32.0);
	}
}
