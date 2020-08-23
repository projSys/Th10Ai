#pragma once

#include "Th10Ai/Th10Data.h"
#include "Th10Ai/Entity.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		static const Pointf INIT_POS;
		static const Pointf MOVE_SPEED_MARISA[DIR_MAXCOUNT];
		static const Pointf MOVE_SPEED_HIGH[DIR_MAXCOUNT];
		static const Pointf MOVE_SPEED_LOW[DIR_MAXCOUNT];

		Player();
		explicit Player(const PlayerData& data);

		void advance(Direction dir, bool slow);
		void checkPrevMove(Direction prevDir, bool prevSlow) const;

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
