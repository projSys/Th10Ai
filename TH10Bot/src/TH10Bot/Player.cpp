#include "TH10Bot/Common.h"
#include "TH10Bot/Player.h"

namespace th
{
	const Pointf Player::INIT_POS = { 0.0f, 400.0f };

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
