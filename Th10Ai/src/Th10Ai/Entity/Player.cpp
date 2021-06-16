#include "Th10Ai/Entity/Player.h"

namespace th
{
	Player::Player() :
		m_status(),
		m_invincibleTime(),
		m_slow(),
		powers(),
		character(),
		lives(),
		itemGetRange()
	{
	}

	void Player::set(const PlayerRaw* raw)
	{
		m_pos.x = raw->x;
		m_pos.y = raw->y;
		m_delta.x = static_cast<float_t>(raw->dx) / _F(100.0);
		m_delta.y = static_cast<float_t>(raw->dy) / _F(100.0);
		m_size.x = raw->width * _F(2.0);
		m_size.y = raw->height * _F(2.0);
		m_status = raw->status;
		m_invincibleTime = raw->invincibleTime;
		m_slow = raw->slow;
	}

	void Player::move(DIR dir, bool slow)
	{
		if (slow)
		{
			vec2 offset = MOVE_SPEED_LOW[to_underlying(dir)];
			m_pos += offset;
			translate(offset);
		}
		else
		{
			if (isMarisa())
			{
				vec2 offset = MOVE_SPEED_MARISA[to_underlying(dir)];
				m_pos += offset;
				translate(offset);
			}
			else
			{
				vec2 offset = MOVE_SPEED_HIGH[to_underlying(dir)];
				m_pos += offset;
				translate(offset);
			}
		}
	}

	bool Player::isRebirthStatus() const
	{
		return m_status == 0;
	}

	bool Player::isNormalStatus() const
	{
		return m_status == 1;
	}

	bool Player::isColliding() const
	{
		return m_status == 4;
	}

	bool Player::isInvincible() const
	{
		return m_invincibleTime != 0;
	}

	bool Player::isSlow() const
	{
		return m_slow != 0;
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
