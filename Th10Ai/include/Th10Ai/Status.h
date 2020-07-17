#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	// ʵ��ṹ
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

		float_t x;			// ���������ĵ�
		float_t y;
		float_t dx;
		float_t dy;
		float_t width;
		float_t height;
	};

	struct Player_t :
		public Entity_t
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

	struct Item_t :
		public Entity_t
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

	struct Enemy_t :
		public Entity_t
	{
	};

	struct Bullet_t :
		public Entity_t
	{
	};

	// ����ṹ
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
