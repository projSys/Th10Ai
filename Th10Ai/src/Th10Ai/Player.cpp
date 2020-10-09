#include "Th10Ai/Player.h"

namespace th
{
	Player::Player() :
		status(),
		invincibleTime(),
		slow(),
		powers(),
		type(),
		lives(),
		itemObtainRange()
	{
	}

	Player::Player(const PlayerData& data) :
		Entity(data),
		status(data.status),
		invincibleTime(data.invincibleTime),
		slow(data.slow),
		powers(data.powers),
		type(data.type),
		lives(data.lives),
		itemObtainRange(data.itemObtainRange)
	{
	}

	void Player::advance(DIR dir, bool slow)
	{
		if (slow)
		{
			x += MOVE_SPEED_LOW[enum_cast(dir)].x;
			y += MOVE_SPEED_LOW[enum_cast(dir)].y;
		}
		else
		{
			if (type == 1)
			{
				x += MOVE_SPEED_MARISA[enum_cast(dir)].x;
				y += MOVE_SPEED_MARISA[enum_cast(dir)].y;
			}
			else
			{
				x += MOVE_SPEED_HIGH[enum_cast(dir)].x;
				y += MOVE_SPEED_HIGH[enum_cast(dir)].y;
			}
		}
	}

	bool Player::isRebirthStatus() const
	{
		return status == 0;
	}

	bool Player::isNormalStatus() const
	{
		return status == 1;
	}

	bool Player::isColliding() const
	{
		return status == 4;
	}

	bool Player::isInvincible() const
	{
		return invincibleTime != 0;
	}
}
