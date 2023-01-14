#include "Th10Base/Player.h"

namespace th
{
	Player::Player() :
		status(),
		invincibleTime(),
		slow()/*,
		powers(),
		character(),
		lives(),
		itemGetRange()*/
	{
	}

	void Player::set(const PlayerRaw* raw)
	{
		pos.x = raw->x;
		pos.y = raw->y;
		delta.x = static_cast<float_t>(raw->dx) / _F(100.0);
		delta.y = static_cast<float_t>(raw->dy) / _F(100.0);
		size.x = raw->width * _F(2.0);
		size.y = raw->height * _F(2.0);
		status = raw->status;
		invincibleTime = raw->invincibleTime;
		slow = raw->slow;
	}

	void Player::move(Direction dir, bool slow)
	{
		if (slow)
		{
			vec2 offset = MOVE_SPEED_LOW[to_underlying(dir)];
			pos += offset;
			translate(offset);
		}
		else
		{
			if (isMarisa())
			{
				vec2 offset = MOVE_SPEED_HIGH_MARISA[to_underlying(dir)];
				pos += offset;
				translate(offset);
			}
			else
			{
				vec2 offset = MOVE_SPEED_HIGH[to_underlying(dir)];
				pos += offset;
				translate(offset);
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
		return character == 0;
	}

	bool Player::isMarisa() const
	{
		return character == 1;
	}
}
