#include "Th10Hook/Common.h"
#include "Th10Hook/Player.h"

namespace th
{
	const Pointf Player::INIT_POS = { 0.0f, 400.0f };

	// 移动速度
	// 3.53 = 5.0 * sqrt(0.5)
	const Pointf Player::MOVE_SPEED_MARISA[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ -5.0f, 0.0f },
		{ 5.0f, 0.0f },
		{ 0.0f, -5.0f },
		{ 0.0f, 5.0f },
		{ -3.53f, -3.53f },
		{ 3.53f, -3.53f },
		{ -3.53f, 3.53f },
		{ 3.53f, 3.53f }
	};

	// 3.18 = 4.5 * sqrt(0.5)
	const Pointf Player::MOVE_SPEED_HIGH[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ -4.5f, 0.0f },
		{ 4.5f, 0.0f },
		{ 0.0f, -4.5f },
		{ 0.0f, 4.5f },
		{ -3.18f, -3.18f },
		{ 3.18f, -3.18f },
		{ -3.18f, 3.18f },
		{ 3.18f, 3.18f }
	};

	// 1.41 = 2.0 * sqrt(0.5)
	const Pointf Player::MOVE_SPEED_LOW[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ -2.0f, 0.0f },
		{ 2.0f, 0.0f },
		{ 0.0f, -2.0f },
		{ 0.0f, 2.0f },
		{ -1.41f, -1.41f },
		{ 1.41f, -1.41f },
		{ -1.41f, 1.41f },
		{ 1.41f, 1.41f }
	};

	Player::Player() :
		status(), invincibleTime(), slow(), powers(), /*type(),*/ life(), itemObtainRange()
	{
	}

	void Player::advance(Direction dir, bool slow)
	{
		if (slow)
		{
			x += MOVE_SPEED_LOW[dir].x;
			y += MOVE_SPEED_LOW[dir].y;
		}
		else
		{
			if (type == 1)
			{
				x += MOVE_SPEED_MARISA[dir].x;
				y += MOVE_SPEED_MARISA[dir].y;
			}
			else
			{
				x += MOVE_SPEED_HIGH[dir].x;
				y += MOVE_SPEED_HIGH[dir].y;
			}
		}
	}

	void Player::checkPrevMove(Direction prevDir, bool prevSlow) const
	{
		Pointf prevDelta;
		if (prevSlow)
		{
			prevDelta = MOVE_SPEED_LOW[prevDir];
		}
		else
		{
			if (type == 1)
			{
				prevDelta = MOVE_SPEED_MARISA[prevDir];
			}
			else
			{
				prevDelta = MOVE_SPEED_HIGH[prevDir];
			}
		}
		if (getDelta() != prevDelta)
			cout << "移动结果错误。" << endl;
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
