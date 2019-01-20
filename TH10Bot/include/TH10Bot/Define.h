#pragma once

namespace th
{
	// ��������
	//     +-----------------------+-----------------------+
	//     | 24  8                                         |16
	//     |----+-+----------------+----------------+-+----|---------> X
	//   (-200,0) (-192,0)         |(0,0)     (192,0) (200,0)
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |448             | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	//     |    | |                |                | |    |
	// (-200,448) (-192,448)       |(0,448) (192,448) (200,448)
	//     |----+-+----------------+----------------+-+----|
	//     |    | |               384               | |    |
	//     +-----------------------+-----------------------+
	//                             |
	//                            \|/
	//                             Y

	const Sizef WINDOW_SIZE = { 640.0f, 480.0f };

	const Pointf SCENE_POS = { 24.0f, 16.0f };

	const Sizef SCENE_SIZE = { 384.0f, 448.0f };

	const Pointf GAME_OFFSET = { 200.0f, 0.0f };

	const Pointf INIT_POS = { 0.0f, 400.0f };



	// ����
	const int_t DIR_HOLD = 0x00;
	const int_t DIR_UP = 0x01;
	const int_t DIR_DOWN = 0x02;
	const int_t DIR_LEFT = 0x04;
	const int_t DIR_RIGHT = 0x08;

	const int_t DIRECTIONS[] =
	{
		DIR_HOLD,				// 0 ����
		DIR_UP,					// 1 ��
		DIR_DOWN,				// 2 ��
		DIR_LEFT,				// 4 ��
		DIR_RIGHT,				// 8 ��
		DIR_UP | DIR_LEFT,		// 5 ����
		DIR_UP | DIR_RIGHT,		// 9 ����
		DIR_DOWN | DIR_LEFT,	// 6 ����
		DIR_DOWN | DIR_RIGHT	// 10 ����
	};

	const int_t NUM_DIRECTIONS = sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0]);

	const float_t SQRT_PF = std::sqrt(0.5f);

	// ��������ƶ�ϵ��
	const Pointf DIR_FACTORS[] =
	{
		{ 0.0f, 0.0f },			// ����
		{ 0.0f, -1.0f },		// ��
		{ 0.0f, 1.0f },			// ��
		{ -1.0f, 0.0f },		// ��
		{ 1.0f, 0.0f },			// ��
		{ -SQRT_PF, -SQRT_PF },	// ����
		{ SQRT_PF, -SQRT_PF },	// ����
		{ -SQRT_PF, SQRT_PF },	// ����
		{ SQRT_PF, SQRT_PF }	// ����
	};

	const int_t NUM_DIR_FACTORS = sizeof(DIR_FACTORS) / sizeof(DIR_FACTORS[0]);

	//static_assert(NUM_DIRECTIONS == NUM_DIR_FACTORS);

	// �ƶ��ٶ�
	const float_t MOVE_SPEEDS[] =
	{
		4.5f,
		2.0f
	};

	const int_t NUM_MOVE_SPEEDS = sizeof(MOVE_SPEEDS) / sizeof(MOVE_SPEEDS[0]);



	// �ü�����ͽǶ�
	const float_t CLIP_DISTANCE = 100.0f;
	const float_t CLIP_ANGLE45 = 45.0f;
	const float_t CLIP_ANGLE90 = 90.0f;
}
