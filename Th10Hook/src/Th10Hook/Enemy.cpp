#include "Th10Hook/Enemy.h"

namespace th
{
	Enemy::Enemy()
	{
	}

	bool Enemy::isBoss() const
	{
		return size == vec2(_F(32.0), _F(32.0));
	}
}
