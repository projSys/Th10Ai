#include "Th10Ai/Entity/Enemy.h"

namespace th
{
	Enemy::Enemy()
	{
	}

	bool Enemy::isBoss() const
	{
		return m_size == vec2(_F(32.0), _F(32.0));
	}
}
