#pragma once

#include "Th10Hook/Common.h"

#include <type_traits>

namespace th
{
	struct PlayerRaw
	{
		byte_t unknown[0x4478];
	};

	static_assert(sizeof(PlayerRaw) == 0x4478, "Size is not correct.");

	// 实体结构
	// +-------+
	// | (x,y) |
	// |   *   | h
	// |       |
	// +-------+
	//     w
	//struct EntityData
	//{
	//	//int32_t id;
	//	//int32_t type;

	//	float32_t x;			// 坐标在中心点
	//	float32_t y;
	//	float32_t dx;
	//	float32_t dy;
	//	float32_t width;
	//	float32_t height;
	//};

	struct PlayerData
	{
		float32_t x;
		float32_t y;
		float32_t dx;
		float32_t dy;
		float32_t width;
		float32_t height;

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

	struct ItemData
	{
		float32_t x;
		float32_t y;
		float32_t dx;
		float32_t dy;
		float32_t width;
		float32_t height;

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
		int32_t type;
	};

	struct EnemyData
	{
		float32_t x;
		float32_t y;
		float32_t dx;
		float32_t dy;
		float32_t width;
		float32_t height;
	};

	struct BulletData
	{
		int32_t id;

		float32_t x;
		float32_t y;
		float32_t dx;
		float32_t dy;
		float32_t width;
		float32_t height;
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
	struct LaserData
	{
		float32_t x;
		float32_t y;
		float32_t dx;
		float32_t dy;
		float32_t width;
		float32_t height;

		float32_t arc;
	};

	struct StatusData
	{
		int64_t frame;
		PlayerData player;
		ItemData items[2000];
		uint_t itemCount;
		EnemyData enemies[2000];
		uint_t enemyCount;
		BulletData bullets[2000];
		uint_t bulletCount;
		LaserData lasers[2000];
		uint_t laserCount;
	};

	static_assert(std::is_trivial_v<StatusData>, "StatusData is not trivial.");
	static_assert(std::is_standard_layout_v<StatusData>, "StatusData is not standard layout.");
	static_assert(std::is_pod_v<StatusData>, "StatusData is not POD.");

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
}
