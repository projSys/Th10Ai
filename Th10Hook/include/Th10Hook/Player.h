#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Entity.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		static constexpr vec2 INIT_POS = { 0, 400 };

		// 3.53 = 5 * sqrt(0.5)
		static constexpr vec2 MOVE_SPEED_MARISA[enum_cast(DIR::MAX_COUNT)] =
		{
			{ 0, 0 },
			{ -5, 0 },
			{ 5, 0 },
			{ 0, -5 },
			{ 0, 5 },
			{ -3.53f, -3.53f },
			{ -3.53f, 3.53f },
			{ 3.53f, -3.53f },
			{ 3.53f, 3.53f }
		};

		// 3.18 = 4.5 * sqrt(0.5)
		static constexpr vec2 MOVE_SPEED_HIGH[enum_cast(DIR::MAX_COUNT)] =
		{
			{ 0, 0 },
			{ -4.5f, 0 },
			{ 4.5f, 0 },
			{ 0, -4.5f },
			{ 0, 4.5f },
			{ -3.18f, -3.18f },
			{ -3.18f, 3.18f },
			{ 3.18f, -3.18f },
			{ 3.18f, 3.18f }
		};

		// 1.41 = 2 * sqrt(0.5)
		static constexpr vec2 MOVE_SPEED_LOW[enum_cast(DIR::MAX_COUNT)] =
		{
			{ 0, 0 },
			{ -2, 0 },
			{ 2, 0 },
			{ 0, -2 },
			{ 0, 2 },
			{ -1.41f, -1.41f },
			{ -1.41f, 1.41f },
			{ 1.41f, -1.41f },
			{ 1.41f, 1.41f }
		};

		Player();

		void advance(DIR dir, bool slow);

		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;
		bool isInvincible() const;

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

		int_t frame;
	};
}
