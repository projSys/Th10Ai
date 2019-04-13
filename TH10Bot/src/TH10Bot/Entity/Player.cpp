#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Player.h"

namespace th
{
	Player Player::move(Direction dir) const
	{
		Player next = *this;
		next.x += MOVE_SPEED[dir].x;
		next.y += MOVE_SPEED[dir].y;
		return next;
	}

	Player Player::moveSlow(Direction dir) const
	{
		Player next = *this;
		next.x += MOVE_SPEED_SLOW[dir].x;
		next.y += MOVE_SPEED_SLOW[dir].y;
		return next;
	}
}
