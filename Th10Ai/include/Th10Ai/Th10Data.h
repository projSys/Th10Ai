#pragma once

#include "Th10Ai/Common.h"

#include <type_traits>

namespace th
{
	// 实体结构
	// +-------+
	// | (x,y) |
	// |   *   | h
	// |       |
	// +-------+
	//     w
	struct RawEntity
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

	struct RawPlayer :
		public RawEntity
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

	struct RawItem :
		public RawEntity
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

	struct RawEnemy :
		public RawEntity
	{
	};

	struct RawBullet :
		public RawEntity
	{
	};

	// 激光结构
	// (x,y)
	// +-*-+
	// |   |
	// |   |
	// |   |
	// |   | h
	// |   |
	// |   |
	// |   |
	// +---+
	//   w
	struct RawLaser :
		public RawEntity
	{
		float_t arc;
	};

	struct RawStatus
	{
		RawPlayer player;
		RawItem items[2000];
		uint_t itemCount;
		RawEnemy enemies[2000];
		uint_t enemyCount;
		RawBullet bullets[2000];
		uint_t bulletCount;
		RawLaser lasers[2000];
		uint_t laserCount;
	};

	static_assert(std::is_trivial_v<RawStatus>, "");
	//static_assert(std::is_standard_layout_v<RawStatus>, "");
	//static_assert(std::is_pod_v<RawStatus>, "");

	struct ActionData
	{
		bool left;
		bool right;
		bool up;
		bool down;
		bool shoot;
		bool bomb;
		bool slow;
		bool skip;
	};

	static_assert(std::is_trivial_v<ActionData>, "ActionData is not trivial.");
	static_assert(std::is_standard_layout_v<ActionData>, "ActionData is not standard layout.");
	static_assert(std::is_pod_v<ActionData>, "ActionData is not POD.");
}
