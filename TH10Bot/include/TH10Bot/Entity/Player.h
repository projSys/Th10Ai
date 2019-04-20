#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	// 移动速度
	// 3.18 = 4.5 * sqrt(0.5)
	const Pointf MOVE_SPEED[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ 0.0f, -4.5f },
		{ 0.0f, 4.5f },
		{ -4.5f, 0.0f },
		{ 4.5f, 0.0f },
		{ -3.18f, -3.18f },
		{ 3.18f, -3.18f },
		{ -3.18f, 3.18f },
		{ 3.18f, 3.18f }
	};

	// 1.41 = 2.0 * sqrt(0.5)
	const Pointf MOVE_SPEED_SLOW[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ 0.0f, -2.0f },
		{ 0.0f, 2.0f },
		{ -2.0f, 0.0f },
		{ 2.0f, 0.0f },
		{ -1.41f, -1.41f },
		{ 1.41f, -1.41f },
		{ -1.41f, 1.41f },
		{ 1.41f, 1.41f }
	};

	struct Player :
		Entity
	{
		Player move(Direction dir) const;
		Player moveSlow(Direction dir) const;

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
