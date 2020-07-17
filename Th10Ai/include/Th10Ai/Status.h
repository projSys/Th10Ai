#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	// 实体结构
	// +-------+
	// | (x,y) |
	// |   *   | h
	// |       |
	// +-------+
	//     w
	struct Entity_t
	{
		//int_t id;
		//int_t type;

		float_t x;			// 坐标在中心点
		float_t y;
		float_t dx;
		float_t dy;
		float_t width;
		float_t height;
	};

	struct Player_t :
		public Entity_t
	{
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

	struct Item_t :
		public Entity_t
	{
		// 正常点分为以下几种
		// type == 1 Power Items P点（红点）
		// type == 2 Point Items 得分点（蓝点）
		// type == 3 Faith Items 信仰点（绿点）
		// type == 4 Large Power Items 大P点（红点）
		// type == 5 Large Point Items 大得分点（带黄色边线的蓝点），由BOSS掉落
		// type == 6 Unknown
		// type == 7 Life Items 续命点（紫点、1UP点）
		// type == 8 Unknown
		// type == 9 Faith Items 信仰点（绿点），满灵力时由P点转化而来
		// type == 10 Power Items P点（红点），由BOSS掉落
		int_t type;
	};

	struct Enemy_t :
		public Entity_t
	{
	};

	struct Bullet_t :
		public Entity_t
	{
	};

	// 激光结构
	//   (x,y)
	// +---*---+
	// |       |
	// |       |
	// |       |
	// |       | h
	// |       |
	// |       |
	// |       |
	// +-------+
	//     w
	struct Laser_t :
		public Entity_t
	{
		float_t arc;
	};

	struct Status_t
	{
		Player_t player;
		Item_t items[2000];
		uint_t itemCount;
		Enemy_t enemies[2000];
		uint_t enemyCount;
		Bullet_t bullets[2000];
		uint_t bulletCount;
		Laser_t lasers[2000];
		uint_t laserCount;
	};
}
