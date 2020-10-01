#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity.h"

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
		explicit Player(const PlayerData& data);

		void advance(DIR dir, bool slow);
		void checkPrevMove(DIR prevDir, bool prevSlow) const;

		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;
		bool isInvincible() const;

		int_t status;
		int_t invincibleTime;
		int_t slow;
		float_t powers;
		int_t type;
		int_t lives;
		float_t itemObtainRange;
	};
}
