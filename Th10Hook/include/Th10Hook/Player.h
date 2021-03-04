#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Entity.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		static constexpr Pointf INIT_POS = { 0.0f, 400.0f };

		// float常量的误差导致碰撞检测不准确？
		// 3.53 = 5.0 * sqrt(0.5)
		static constexpr Pointf MOVE_SPEED_MARISA[enum_cast(DIR::MAX_COUNT)] =
		{
			{ 0.0f, 0.0f },
			{ -5.0f, 0.0f },
			{ 5.0f, 0.0f },
			{ 0.0f, -5.0f },
			{ 0.0f, 5.0f },
			{ -3.53f, -3.53f },
			{ -3.53f, 3.53f },
			{ 3.53f, -3.53f },
			{ 3.53f, 3.53f }
		};

		// 3.18 = 4.5 * sqrt(0.5)
		static constexpr Pointf MOVE_SPEED_HIGH[enum_cast(DIR::MAX_COUNT)] =
		{
			{ 0.0f, 0.0f },
			{ -4.5f, 0.0f },
			{ 4.5f, 0.0f },
			{ 0.0f, -4.5f },
			{ 0.0f, 4.5f },
			{ -3.18f, -3.18f },
			{ -3.18f, 3.18f },
			{ 3.18f, -3.18f },
			{ 3.18f, 3.18f }
		};

		// 1.41 = 2.0 * sqrt(0.5)
		static constexpr Pointf MOVE_SPEED_LOW[enum_cast(DIR::MAX_COUNT)] =
		{
			{ 0.0f, 0.0f },
			{ -2.0f, 0.0f },
			{ 2.0f, 0.0f },
			{ 0.0f, -2.0f },
			{ 0.0f, 2.0f },
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
		int32_t status;
		// 剩余无敌时间
		int32_t invincibleTime;
		// slow == 0 High Speed
		// slow == 1 Low Speed
		int32_t slow;
		float32_t powers;
		// type == 0 Reimu
		// type == 1 Marisa
		int32_t type;
		int32_t lives;
		float32_t itemObtainRange;
	};
}
