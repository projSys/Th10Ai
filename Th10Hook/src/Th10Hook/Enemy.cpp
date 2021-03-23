#include "Th10Hook/Enemy.h"

namespace th
{
	Enemy::Enemy()
	{
	}

	bool Enemy::isBoss() const
	{
		return size == vec2(32, 32);
	}
}
