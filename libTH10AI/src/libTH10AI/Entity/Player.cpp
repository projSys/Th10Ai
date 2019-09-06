#include "libTH10AI/Common.h"
#include "libTH10AI/Entity/Player.h"

namespace th
{
	const Pointf Player::INIT_POS = { 0.0f, 400.0f };

	// ÒÆ¶¯ËÙ¶È
	// 3.18 = 4.5 * sqrt(0.5)
	const Pointf Player::MOVE_SPEED[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ 0.0f, -4.5f },
		{ 0.0f, 4.5f },
		{ -4.5f, 0.0f },
		{ 4.5f, 0.0f },
		{ -3.18f, -3.18f },
		{ 3.18f, -3.18f },
		{ -3.18f, 3.18f },
		{ 3.18f, 3.18f }
	};

	// 1.41 = 2.0 * sqrt(0.5)
	const Pointf Player::MOVE_SPEED_SLOW[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ 0.0f, -2.0f },
		{ 0.0f, 2.0f },
		{ -2.0f, 0.0f },
		{ 2.0f, 0.0f },
		{ -1.41f, -1.41f },
		{ 1.41f, -1.41f },
		{ -1.41f, 1.41f },
		{ 1.41f, 1.41f }
	};

	void Player::advance(Direction dir, bool slow)
	{
		if (slow)
		{
			x += MOVE_SPEED_SLOW[dir].x;
			y += MOVE_SPEED_SLOW[dir].y;
		}
		else
		{
			x += MOVE_SPEED[dir].x;
			y += MOVE_SPEED[dir].y;
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
}
