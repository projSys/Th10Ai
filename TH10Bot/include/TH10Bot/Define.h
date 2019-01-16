#pragma once

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

	const float_t SCENE_WIDTH = 400;
	const float_t SCENE_HEIGHT = 480;

	const Pointf SCENE_OFFSET = { 24, 16 };

	const Pointf POS_OFFSET = { 200, 0 };

	// ��ʼλ��
	const Pointf INIT_POS = { 0, 400 };



	// ����
	const int_t DIR_CENTER = 0x00;
	const int_t DIR_UP = 0x01;
	const int_t DIR_DOWN = 0x02;
	const int_t DIR_LEFT = 0x04;
	const int_t DIR_RIGHT = 0x08;

	const int_t DIRECTION[] =
	{
		DIR_UP | DIR_LEFT,		// 5 ����
		DIR_UP,					// 1 ��
		DIR_UP | DIR_RIGHT,		// 9 ����
		DIR_LEFT,				// 4 ��
		DIR_CENTER,				// 0 ��
		DIR_RIGHT,				// 8 ��
		DIR_DOWN | DIR_LEFT,	// 6 ����
		DIR_DOWN,				// 2 ��
		DIR_DOWN | DIR_RIGHT	// 10 ����
	};

	const int_t DIR_LEN = sizeof(DIRECTION) / sizeof(DIRECTION[0]);



	const float_t SQRT_PF = std::sqrt(0.5f);

	// ��������ƶ�ϵ��
	const Pointf DIR_FACTOR[] =
	{
		{ -SQRT_PF, -SQRT_PF },	// ����
		{ 0.0, -1.0 },			// ��
		{ SQRT_PF, -SQRT_PF },	// ����
		{ -1.0, 0.0 },			// ��
		{ 0.0, 0.0 },			// ��
		{ 1.0, 0.0 },			// ��
		{ -SQRT_PF, SQRT_PF },	// ����
		{ 0.0, 1.0 },			// ��
		{ SQRT_PF, SQRT_PF }	// ����
	};

	const int_t DIR_FACTOR_LEN = sizeof(DIR_FACTOR) / sizeof(DIR_FACTOR[0]);

	//static_assert(DIR_LEN == DIR_FACTOR_LEN);

	// �ƶ��ٶ�
	const float_t MOVE_SPEED[] =
	{
		4.5,
		2.0
	};

	const int_t MOVE_SPEED_LEN = sizeof(MOVE_SPEED) / sizeof(MOVE_SPEED[0]);



	// �����������룬��Ϊ���ַ�ĸ 624.81
	const float_t SCENE_MAXDIST = std::sqrt(SCENE_WIDTH * SCENE_WIDTH + SCENE_HEIGHT * SCENE_HEIGHT);



	// �ü��ǶȺ;���
	const float_t CLIP_ANGLE = 45;
	const float_t CLIP_DISTANCE = 100;
}
