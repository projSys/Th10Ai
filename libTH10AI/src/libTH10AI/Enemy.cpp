#include "libTH10AI/Common.h"
#include "libTH10AI/Enemy.h"

namespace th
{
	bool Enemy::isBoss() const
	{
		return getSize() == Sizef(32.0f, 32.0f);
	}
}
