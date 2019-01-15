#pragma once

#include "TH10Bot/Point2.h"
#include "TH10Bot/Rect2.h"

namespace th
{
	// ��������
	//(-200,0)        (0,0)      (200,0)
	//   +-----------+-----------+ X
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   +-----------+-----------+
	//(-200,480)     Y(0,480)    (200,480)

	const double SCENE_WIDTH = 400.0;
	const double SCENE_HEIGHT = 480.0;

	// ��ʼλ��
	const Point2d INIT_POS = { 0.0, 400.0 };
	const Rect2d INIT_RECT = { 0.0, 400.0, 4.0, 4.0 };



	// ����
	const int DIR_CENTER = 0x00;
	const int DIR_UP = 0x01;
	const int DIR_DOWN = 0x02;
	const int DIR_LEFT = 0x04;
	const int DIR_RIGHT = 0x08;

	const int DIRECTION[] =
	{
		DIR_CENTER,				// ��
		DIR_UP | DIR_LEFT,		// ����
		DIR_UP,					// ��
		DIR_UP | DIR_RIGHT,		// ����
		DIR_LEFT,				// ��
		DIR_RIGHT,				// ��
		DIR_DOWN | DIR_LEFT,	// ����
		DIR_DOWN,				// ��
		DIR_DOWN | DIR_RIGHT	// ����
	};

	const int DIR_LEN = sizeof(DIRECTION) / sizeof(DIRECTION[0]);

	const double SQRT_PF = std::sqrt(0.5);

	// ��������ƶ�ϵ��
	const Point2d DIR_FACTOR[] =
	{
		{ 0.0, 0.0 },			// ��
		{ -SQRT_PF, -SQRT_PF },	// ����
		{ 0.0, -1.0 },			// ��
		{ SQRT_PF, -SQRT_PF },	// ����
		{ -1.0, 0.0 },			// ��
		{ 1.0, 0.0 },			// ��
		{ -SQRT_PF, SQRT_PF },	// ����
		{ 0.0, 1.0 },			// ��
		{ SQRT_PF, SQRT_PF }	// ����
	};

	const int DIR_FACTOR_LEN = sizeof(DIR_FACTOR) / sizeof(DIR_FACTOR[0]);

	//static_assert(DIR_LEN == DIR_FACTOR_LEN);

	// �ƶ��ٶ�
	const double MOVE_SPEED[] =
	{
		4.5,
		2.0
	};

	const int MOVE_SPEED_LEN = sizeof(MOVE_SPEED) / sizeof(MOVE_SPEED[0]);



	// �����������룬��Ϊ���ַ�ĸ 624.81
	const double SCENE_MAXDIST = std::sqrt(SCENE_WIDTH * SCENE_WIDTH + SCENE_HEIGHT * SCENE_HEIGHT);
}
