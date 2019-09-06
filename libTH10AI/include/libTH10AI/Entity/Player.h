#pragma once

#include "libTH10AI/Entity/Entity.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		static const Pointf INIT_POS;

		void advance(Direction dir, bool slow);

		bool isRebirthStatus() const;
		bool isNormalStatus() const;
		bool isColliding() const;

		static const Pointf MOVE_SPEED[DIR_MAXCOUNT];
		static const Pointf MOVE_SPEED_SLOW[DIR_MAXCOUNT];

		//float_t powers;
		//int_t life;
		//// type == 0 Reimu
		//// type == 1 Marisa
		//int_t type;
		//// slow == 0 High Speed
		//// slow == 1 Low Speed
		//int_t slow;
		//float_t itemObtainRange;
		//// status == 0 重生状态，此时无敌
		//// status == 1 正常状态
		//// status == 2 死亡
		//// status == 3 Unknown
		//// status == 4 被弹、决死，此时放B可以决死
		int_t status;
		//// 剩余无敌时间
		//int_t invinibleTime;
	};
}
