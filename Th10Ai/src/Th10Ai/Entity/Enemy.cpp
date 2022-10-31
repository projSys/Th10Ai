#include "Th10Ai/Entity/Enemy.h"

namespace th
{
	Enemy::Enemy()
	{
	}

	Enemy::Enemy(const EnemyRaw* raw)
	{
		pos.x = raw->x;
		pos.y = raw->y;
		delta.x = raw->dx;
		delta.y = raw->dy;
		size.x = raw->width;
		size.y = raw->height;
	}

	bool Enemy::isBoss() const
	{
		return size == vec2(_F(32.0), _F(32.0));
	}
}
