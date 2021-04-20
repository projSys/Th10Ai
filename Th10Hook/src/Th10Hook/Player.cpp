#include "Th10Hook/Player.h"

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

	void Player::advance(DIR dir, bool slow)
	{
		if (slow)
		{
			pos += MOVE_SPEED_LOW[enum_cast(dir)];
		}
		else
		{
			if (isMarisa())
			{
				pos += MOVE_SPEED_MARISA[enum_cast(dir)];
			}
			else
			{
				pos += MOVE_SPEED_HIGH[enum_cast(dir)];
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

	bool Player::isSlow() const
	{
		return slow != 0;
	}

	bool Player::isReimu() const
	{
		return type == 0;
	}

	bool Player::isMarisa() const
	{
		return type == 1;
	}
}
