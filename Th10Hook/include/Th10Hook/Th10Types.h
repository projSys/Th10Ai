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

	// ʵ��ṹ
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

	//	float32_t x;			// ���������ĵ�
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

		// status == 0 ����״̬����ʱ�޵�
		// status == 1 ����״̬
		// status == 2 ����
		// status == 3 Unknown
		// status == 4 ��������������ʱ��B���Ծ���
		int32_t status;
		// ʣ���޵�ʱ��
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

		// �������Ϊ���¼���
		// type == 1 Power Items P�㣨��㣩
		// type == 2 Point Items �÷ֵ㣨���㣩
		// type == 3 Faith Items �����㣨�̵㣩
		// type == 4 Large Power Items ��P�㣨��㣩
		// type == 5 Large Point Items ��÷ֵ㣨����ɫ���ߵ����㣩����BOSS����
		// type == 6 Unknown
		// type == 7 Life Items �����㣨�ϵ㡢1UP�㣩
		// type == 8 Unknown
		// type == 9 Faith Items �����㣨�̵㣩��������ʱ��P��ת������
		// type == 10 Power Items P�㣨��㣩����BOSS����
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

	// ����ṹ
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
