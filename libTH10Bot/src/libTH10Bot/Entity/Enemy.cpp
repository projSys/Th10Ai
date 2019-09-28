#include "libTH10Bot/Common.h"
#include "libTH10Bot/Entity/Enemy.h"

namespace th
{
	bool Enemy::isBoss() const
	{
		return getSize() == Sizef(32.0f, 32.0f);
	}
}
