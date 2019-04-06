#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	class Player :
		public Entity
	{
	public:
		//Player() :
		//	powers(), life(), type(), slow(), itemObtainRange(), status(), invinibleTime() {}
		//Player(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
		//	Entity(x0, y0, dx0, dy0, width0, height0),
		//	powers(), life(), type(), slow(), itemObtainRange(), status(), invinibleTime() {}

		float_t powers;
		int_t life;
		// type == 0 Reimu
		// type == 1 Marisa
		int_t type;
		// slow == 0 High Speed
		// slow == 1 Low Speed
		int_t slow;
		float_t itemObtainRange;
		// status == 0 重生状态，此时无敌
		// status == 1 正常状态
		// status == 2 死亡
		// status == 3 Unknown
		// status == 4 被弹、决死，此时放B可以决死
		int_t status;
		// 剩余无敌时间
		int_t invinibleTime;
	};
}
