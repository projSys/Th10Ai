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

	const Sizef SCENE_SIZE = { 400.0f, 480.0f };

	const Pointf SCENE_OFFSET = { 24.0f, 16.0f };

	const Pointf POS_OFFSET = { 200.0f, 0.0f };

	// ��ʼλ��
	const Pointf INIT_POS = { 0.0f, 400.0f };



	// ����
	const int_t DIR_CENTER = 0x00;
	const int_t DIR_UP = 0x01;
	const int_t DIR_DOWN = 0x02;
	const int_t DIR_LEFT = 0x04;
	const int_t DIR_RIGHT = 0x08;

	const int_t DIRECTIONS[] =
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

	const int_t DIRECTIONS_LEN = sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0]);



	const float_t SQRT_PF = std::sqrt(0.5f);

	// ��������ƶ�ϵ��
	const Pointf DIR_FACTORS[] =
	{
		{ -SQRT_PF, -SQRT_PF },	// ����
		{ 0.0f, -1.0f },		// ��
		{ SQRT_PF, -SQRT_PF },	// ����
		{ -1.0f, 0.0f },		// ��
		{ 0.0f, 0.0f },			// ��
		{ 1.0f, 0.0f },			// ��
		{ -SQRT_PF, SQRT_PF },	// ����
		{ 0.0f, 1.0f },			// ��
		{ SQRT_PF, SQRT_PF }	// ����
	};

	const int_t DIR_FACTORS_LEN = sizeof(DIR_FACTORS) / sizeof(DIR_FACTORS[0]);

	//static_assert(DIRECTIONS_LEN == DIR_FACTORS_LEN);

	// �ƶ��ٶ�
	const float_t MOVE_SPEEDS[] =
	{
		4.5f,
		2.0f
	};

	const int_t MOVE_SPEEDS_LEN = sizeof(MOVE_SPEEDS) / sizeof(MOVE_SPEEDS[0]);



	// �ü�����ͽǶ�
	const float_t CLIP_ANGLE = 45.0f;
	const float_t CLIP_DISTANCE = 100.0f;
}
