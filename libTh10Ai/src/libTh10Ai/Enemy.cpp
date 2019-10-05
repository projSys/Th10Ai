#include "libTh10Ai/Common.h"
#include "libTh10Ai/Enemy.h"

namespace th
{
	bool Enemy::isBoss() const
	{
		return getSize() == Sizef(32.0f, 32.0f);
	}
}
