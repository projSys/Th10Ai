#pragma once

namespace th
{
	// 场景坐标
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



	// 方向
	const int_t DIR_HOLD = 0x00;
	const int_t DIR_UP = 0x01;
	const int_t DIR_DOWN = 0x02;
	const int_t DIR_LEFT = 0x04;
	const int_t DIR_RIGHT = 0x08;

	const int_t DIRECTIONS[] =
	{
		DIR_HOLD,				// 0 不动
		DIR_UP,					// 1 上
		DIR_DOWN,				// 2 下
		DIR_LEFT,				// 4 左
		DIR_RIGHT,				// 8 右
		DIR_UP | DIR_LEFT,		// 5 左上
		DIR_UP | DIR_RIGHT,		// 9 右上
		DIR_DOWN | DIR_LEFT,	// 6 左下
		DIR_DOWN | DIR_RIGHT	// 10 右下
	};

	const int_t NUM_DIRECTIONS = sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0]);

	const float_t SQRT_PF = std::sqrt(0.5f);

	// 各方向的移动系数
	const Pointf DIR_FACTORS[] =
	{
		{ 0.0f, 0.0f },			// 不动
		{ 0.0f, -1.0f },		// 上
		{ 0.0f, 1.0f },			// 下
		{ -1.0f, 0.0f },		// 左
		{ 1.0f, 0.0f },			// 右
		{ -SQRT_PF, -SQRT_PF },	// 左上
		{ SQRT_PF, -SQRT_PF },	// 右上
		{ -SQRT_PF, SQRT_PF },	// 左下
		{ SQRT_PF, SQRT_PF }	// 右下
	};

	const int_t NUM_DIR_FACTORS = sizeof(DIR_FACTORS) / sizeof(DIR_FACTORS[0]);

	//static_assert(NUM_DIRECTIONS == NUM_DIR_FACTORS);

	// 移动速度
	const float_t MOVE_SPEEDS[] =
	{
		4.5f,
		2.0f
	};

	const int_t NUM_MOVE_SPEEDS = sizeof(MOVE_SPEEDS) / sizeof(MOVE_SPEEDS[0]);



	// 裁剪距离和角度
	const float_t CLIP_DISTANCE = 100.0f;
	const float_t CLIP_ANGLE45 = 45.0f;
	const float_t CLIP_ANGLE90 = 90.0f;
}
