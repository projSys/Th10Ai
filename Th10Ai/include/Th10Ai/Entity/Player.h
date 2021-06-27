#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/Entity.h"
#include "Th10Ai/Th10Raw.h"
#include "Th10Ai/Direction.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		static constexpr vec2 INIT_POS = vec2(_F(0.0), _F(400.0));

		// 3.53 = 5 * sqrt(0.5)
		static constexpr vec2 MOVE_SPEED_HIGH_MARISA[to_underlying(DIR::MAX_COUNT)] =
		{
			vec2(_F(0.0), _F(0.0)),
			vec2(_F(0.0), _F(-5.0)),
			vec2(_F(0.0), _F(5.0)),
			vec2(_F(-5.0), _F(0.0)),
			vec2(_F(5.0), _F(0.0)),
			vec2(_F(-3.53), _F(-3.53)),
			vec2(_F(3.53), _F(-3.53)),
			vec2(_F(-3.53), _F(3.53)),
			vec2(_F(3.53), _F(3.53))
		};

		// 3.18 = 4.5 * sqrt(0.5)
		static constexpr vec2 MOVE_SPEED_HIGH[to_underlying(DIR::MAX_COUNT)] =
		{
			vec2(_F(0.0), _F(0.0)),
			vec2(_F(0.0), _F(-4.5)),
			vec2(_F(0.0), _F(4.5)),
			vec2(_F(-4.5), _F(0.0)),
			vec2(_F(4.5), _F(0.0)),
			vec2(_F(-3.18), _F(-3.18)),
			vec2(_F(3.18), _F(-3.18)),
			vec2(_F(-3.18), _F(3.18)),
			vec2(_F(3.18), _F(3.18))
		};

		// 1.41 = 2 * sqrt(0.5)
		static constexpr vec2 MOVE_SPEED_LOW[to_underlying(DIR::MAX_COUNT)] =
		{
			vec2(_F(0.0), _F(0.0)),
			vec2(_F(0.0), _F(-2.0)),
			vec2(_F(0.0), _F(2.0)),
			vec2(_F(-2.0), _F(0.0)),
			vec2(_F(2.0), _F(0.0)),
			vec2(_F(-1.41), _F(-1.41)),
			vec2(_F(1.41), _F(-1.41)),
			vec2(_F(-1.41), _F(1.41)),
			vec2(_F(1.41), _F(1.41))
		};

		Player();
		void set(const PlayerRaw* raw);

		void move(DIR dir, bool slow);

		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;
		bool isInvincible() const;
		bool isSlow() const;
		bool isReimu() const;
		bool isMarisa() const;

	//private:
		// status == 0 ����״̬����ʱ�޵�
		// status == 1 ����״̬
		// status == 2 ����
		// status == 3 Unknown
		// status == 4 ��������������ʱ��B���Ծ���
		int_t m_status;
		// ʣ���޵�ʱ��
		int_t m_invincibleTime;
		// slow == 0 High Speed
		// slow == 1 Low Speed
		int_t m_slow;

		float_t powers;
		// character == 0 Reimu
		// character == 1 Marisa
		int_t character;
		int_t lives;
		float_t itemGetRange;

		int_t stageFrame;
	};
}
