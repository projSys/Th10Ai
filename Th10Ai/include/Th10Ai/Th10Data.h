#pragma once

#include "Th10Ai/Common.h"

#include <type_traits>

namespace th
{
	// ʵ��ṹ
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

		float_t x;			// ���������ĵ�
		float_t y;
		float_t dx;
		float_t dy;
		float_t width;
		float_t height;
	};

	struct RawPlayer :
		public RawEntity
	{
		// status == 0 ����״̬����ʱ�޵�
		// status == 1 ����״̬
		// status == 2 ����
		// status == 3 Unknown
		// status == 4 ��������������ʱ��B���Ծ���
		int_t status;
		// ʣ���޵�ʱ��
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
