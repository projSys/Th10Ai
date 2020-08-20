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

		Player() = default;
		Player(const Player&) = default;
		explicit Player(const RawPlayer& raw);

		void advance(Direction dir, bool slow);
		void checkPrevMove(Direction prevDir, bool prevSlow) const;

		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;
		bool isInvincible() const;

		static const Pointf MOVE_SPEED_MARISA[DIR_MAXCOUNT];
		static const Pointf MOVE_SPEED_HIGH[DIR_MAXCOUNT];
		static const Pointf MOVE_SPEED_LOW[DIR_MAXCOUNT];

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
	};

	static_assert(std::is_trivial_v<Player>, "");
	//static_assert(std::is_standard_layout_v<Player>, "");
	//static_assert(std::is_pod_v<Player>, "");
}
