#pragma once

#include "Th10Ai/Common.h"

#include <array>

#include "Th10Ai/Th10Data.h"
#include "Th10Ai/Entity.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		static constexpr Pointf INIT_POS = { 0.0f, 400.0f };

		// 3.53 = 5.0 * sqrt(0.5)
		static constexpr std::array<Pointf, g_dirs.size()> MOVE_SPEED_MARISA =
		{ {
			{ 0.0f, 0.0f },
			{ -5.0f, 0.0f },
			{ 5.0f, 0.0f },
			{ 0.0f, -5.0f },
			{ 0.0f, 5.0f },
			{ -3.53f, -3.53f },
			{ 3.53f, -3.53f },
			{ -3.53f, 3.53f },
			{ 3.53f, 3.53f }
		} };

		// 3.18 = 4.5 * sqrt(0.5)
		static constexpr std::array<Pointf, g_dirs.size()> MOVE_SPEED_HIGH =
		{ {
			{ 0.0f, 0.0f },
			{ -4.5f, 0.0f },
			{ 4.5f, 0.0f },
			{ 0.0f, -4.5f },
			{ 0.0f, 4.5f },
			{ -3.18f, -3.18f },
			{ 3.18f, -3.18f },
			{ -3.18f, 3.18f },
			{ 3.18f, 3.18f }
		} };

		// 1.41 = 2.0 * sqrt(0.5)
		static constexpr std::array<Pointf, g_dirs.size()> MOVE_SPEED_LOW =
		{ {
			{ 0.0f, 0.0f },
			{ -2.0f, 0.0f },
			{ 2.0f, 0.0f },
			{ 0.0f, -2.0f },
			{ 0.0f, 2.0f },
			{ -1.41f, -1.41f },
			{ 1.41f, -1.41f },
			{ -1.41f, 1.41f },
			{ 1.41f, 1.41f }
		} };

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
