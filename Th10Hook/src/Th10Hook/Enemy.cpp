#include "Th10Hook/Enemy.h"

namespace th
{
	Enemy::Enemy()
	{
	}

	Enemy::Enemy(const EnemyData& data) :
		Entity(data)
	{
	}

	bool Enemy::isBoss() const
	{
		return getSize() == Sizef(32.0f, 32.0f);
	}
}
