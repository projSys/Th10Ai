#include "Th10Hook/Common.h"
#include "Th10Hook/Enemy.h"

namespace th
{
	bool Enemy::isBoss() const
	{
		return getSize() == Sizef(32.0f, 32.0f);
	}
}
