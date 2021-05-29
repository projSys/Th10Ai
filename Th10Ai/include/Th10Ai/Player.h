#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity.h"
#include "Th10Ai/Direction.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		static constexpr vec2 INIT_POS = vec2(_F(0.0), _F(400.0));

		// 3.53 = 5 * sqrt(0.5)
		static constexpr vec2 MOVE_SPEED_MARISA[to_underlying(DIR::MAX_COUNT)] =
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

		void advance(DIR dir, bool slow);

		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;
		bool isInvincible() const;
		bool isSlow() const;
		bool isReimu() const;
		bool isMarisa() const;

		// status == 0 重生状态，此时无敌
		// status == 1 正常状态
		// status == 2 死亡
		// status == 3 Unknown
		// status == 4 被弹、决死，此时放B可以决死
		int_t status;
		// 剩余无敌时间
		int_t invincibleTime;
		// slow == 0 High Speed
		// slow == 1 Low Speed
		int_t slow;
		float_t powers;
		// type == 0 Reimu
		// type == 1 Marisa
		int_t type;
		int_t lives;
		float_t itemObtainRange;

		int_t stageFrame;
	};
}
